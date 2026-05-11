#include "Database.hpp"
#include "MenuFuncionarioWindow.h"
#include "CadastroDialog.h"
#include "TriagemDialog.h"
#include "SolicitacoesDialog.h"
#include "EditarDadosDialog.h"
#include "NaseStyle.h"
#include "Paciente.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QMessageBox>
#include <QInputDialog>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>

MenuFuncionarioWindow::MenuFuncionarioWindow(Usuario* usuario, std::vector<Usuario*>& usuarios,
                                             FilaPrioridade& fila, Database* db, QWidget* parent)
    : QWidget(parent), usuario(usuario), usuarios(usuarios), fila(fila), db(db) {
    setWindowTitle("Portal do Atendente – NASE UFPE");
    setMinimumSize(1000, 680);
    resize(1080, 720);

    auto* root = new QHBoxLayout(this);
    root->setContentsMargins(0,0,0,0);
    root->setSpacing(0);

    // ══════════════════════════════
    //  SIDEBAR
    // ══════════════════════════════
    auto* sidebar = new QWidget(this);
    sidebar->setFixedWidth(240);
    sidebar->setObjectName("sidebar");
    auto* sideL = new QVBoxLayout(sidebar);
    sideL->setContentsMargins(14,20,14,20);
    sideL->setSpacing(4);

    // ── Avatar circular ──
    auto* avatarContainer = new QWidget(sidebar);
    avatarContainer->setFixedHeight(160);
    auto* avatarLayout = new QVBoxLayout(avatarContainer);
    avatarLayout->setContentsMargins(0,0,0,0);
    avatarLayout->setSpacing(6);
    avatarLayout->setAlignment(Qt::AlignCenter);
    avatarContainer->setStyleSheet("background: transparent;");

    auto* lblAvatar = new QLabel(avatarContainer);
    lblAvatar->setFixedSize(120, 120);
    lblAvatar->setAlignment(Qt::AlignCenter);

    // Carrega e recorta avatar em círculo
    QPixmap avatarPix(":/nase_avatar.png");
    if (!avatarPix.isNull()) {
        QPixmap scaled = avatarPix.scaled(120, 120, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        QPixmap circle(120, 120);
        circle.fill(Qt::transparent);
        QPainter painter(&circle);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addEllipse(0, 0, 120, 120);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, scaled);
        // Borda verde
        painter.setClipping(false);
        QPen pen(QColor("#4ade80"), 3);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(1, 1, 118, 118);
        lblAvatar->setPixmap(circle);
    } else {
        lblAvatar->setText("👨‍⚕️");
        lblAvatar->setStyleSheet("font-size:48px;");
    }

    auto* lblRole = new QLabel("FUNCIONÁRIO", avatarContainer);
    lblRole->setObjectName("lblSideTitle");
    lblRole->setAlignment(Qt::AlignCenter);
    auto* lblUserName = new QLabel(QString::fromStdString(usuario->getLogin()), avatarContainer);
    lblUserName->setObjectName("lblSideUser");
    lblUserName->setAlignment(Qt::AlignCenter);

    avatarLayout->addWidget(lblAvatar, 0, Qt::AlignCenter);
    avatarLayout->addWidget(lblRole);
    avatarLayout->addWidget(lblUserName);

    auto* sep = new QFrame(sidebar);
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color: rgba(255,255,255,0.25); margin: 4px 0;");

    auto mkSideBtn = [&](const QString& ico, const QString& txt) {
        auto* b = new QPushButton(ico + "  " + txt, sidebar);
        b->setObjectName("btnMenu");
        b->setCheckable(true);
        b->setMinimumHeight(44);
        b->setCursor(Qt::PointingHandCursor);
        return b;
    };

    btnDash   = mkSideBtn("📊", "Dashboard");
    btnSolics = mkSideBtn("📩", "Solicitações");
    btnTriag  = mkSideBtn("🩺", "Triagem");
    btnFila   = mkSideBtn("🏆", "Filas / Ordem");
    btnCad    = mkSideBtn("👤", "Cadastrar aluno");
    btnHist   = mkSideBtn("📋", "Ver histórico");
    btnEdit   = mkSideBtn("✏️",  "Editar dados");

    auto* btnSair = new QPushButton("  ← Sair", sidebar);
    btnSair->setObjectName("btnSideExit");
    btnSair->setMinimumHeight(42);
    btnSair->setCursor(Qt::PointingHandCursor);

    sideL->addWidget(avatarContainer);
    sideL->addWidget(sep);
    sideL->addSpacing(6);
    sideL->addWidget(btnDash);
    sideL->addWidget(btnSolics);
    sideL->addWidget(btnTriag);
    sideL->addWidget(btnFila);
    sideL->addWidget(btnCad);
    sideL->addWidget(btnHist);
    sideL->addWidget(btnEdit);
    sideL->addStretch();
    sideL->addWidget(btnSair);

    // ══════════════════════════════
    //  STACK DE CONTEÚDO
    // ══════════════════════════════
    stackContent = new QStackedWidget(this);

    auto* pgDash  = new QWidget(); buildPageDashboard(pgDash);
    auto* pgSolic = new QWidget(); buildPageSolicitacoes(pgSolic);
    auto* pgTriag = new QWidget(); buildPageTriagem(pgTriag);
    auto* pgFila  = new QWidget(); buildPageFila(pgFila);
    auto* pgCad   = new QWidget(); buildPageCadastro(pgCad);
    auto* pgHist  = new QWidget(); buildPageHistorico(pgHist);
    auto* pgEdit  = new QWidget(); buildPageEditar(pgEdit);

    stackContent->addWidget(pgDash);   // 0
    stackContent->addWidget(pgSolic);  // 1
    stackContent->addWidget(pgTriag);  // 2
    stackContent->addWidget(pgFila);   // 3
    stackContent->addWidget(pgCad);    // 4
    stackContent->addWidget(pgHist);   // 5
    stackContent->addWidget(pgEdit);   // 6

    root->addWidget(sidebar);
    root->addWidget(stackContent);

    auto go = [=](QPushButton* active, int idx) {
        for (auto* b : std::vector<QPushButton*>{btnDash,btnSolics,btnTriag,btnFila,btnCad,btnHist,btnEdit})
            b->setChecked(b == active);
        stackContent->setCurrentIndex(idx);
        if (idx == 0) atualizarDashboard();
        if (idx == 1) atualizarSolicitacoes();
        if (idx == 3) atualizarFila();
        if (idx == 5) atualizarHistorico();
    };

    connect(btnDash,   &QPushButton::clicked, this, [=](){ go(btnDash,   0); });
    connect(btnSolics, &QPushButton::clicked, this, [=](){ go(btnSolics, 1); });
    connect(btnTriag,  &QPushButton::clicked, this, [=](){ go(btnTriag,  2); });
    connect(btnFila,   &QPushButton::clicked, this, [=](){ go(btnFila,   3); });
    connect(btnCad,    &QPushButton::clicked, this, [=](){ go(btnCad,    4); });
    connect(btnHist,   &QPushButton::clicked, this, [=](){ go(btnHist,   5); });
    connect(btnEdit,   &QPushButton::clicked, this, [=](){ go(btnEdit,   6); });
    connect(btnSair,   &QPushButton::clicked, this, &MenuFuncionarioWindow::onVoltar);

    btnDash->setChecked(true);
    atualizarDashboard();
    applyStyle();
}

// ── helpers ──────────────────────────────────────────────────────────────────

QWidget* MenuFuncionarioWindow::makeCard(QWidget* parent, const QString& titulo) {
    auto* card = new QWidget(parent);
    card->setObjectName("card");
    auto* lay = new QVBoxLayout(card);
    lay->setContentsMargins(20,16,20,16);
    lay->setSpacing(10);
    if (!titulo.isEmpty()) {
        auto* lbl = new QLabel(titulo, card);
        lbl->setObjectName("lblCardTit");
        lay->addWidget(lbl);
    }
    return card;
}

QLabel* MenuFuncionarioWindow::makeStat(QWidget* parent,
                                        const QString& num, const QString& desc, const QString& bg) {
    auto* w = new QWidget(parent);
    w->setObjectName("statCard");
    w->setStyleSheet("QWidget#statCard { background:" + bg + "; border-radius:10px; }");
    auto* lay = new QVBoxLayout(w);
    lay->setContentsMargins(14,12,14,12);
    lay->setSpacing(4);
    auto* ln = new QLabel(num, w);
    ln->setStyleSheet("font-size:30px; font-weight:900; color:white; background:transparent;");
    ln->setAlignment(Qt::AlignCenter);
    auto* ld = new QLabel(desc, w);
    ld->setStyleSheet("font-size:11px; color:black; font-weight:800; letter-spacing:1px;");
    ld->setAlignment(Qt::AlignCenter);
    lay->addWidget(ln);
    lay->addWidget(ld);
    return ln;
}

// ── Dashboard ─────────────────────────────────────────────────────────────────

void MenuFuncionarioWindow::buildPageDashboard(QWidget* pg) {
    auto* lay = new QVBoxLayout(pg);
    lay->setContentsMargins(28,24,28,24);
    lay->setSpacing(18);

    auto* hdr = new QLabel("Dashboard", pg);
    hdr->setObjectName("lblPgTit");
    auto* sub = new QLabel("Visão geral do atendimento hoje.", pg);
    sub->setObjectName("lblPgSub");

    // Cards stats
    auto* statsRow = new QHBoxLayout();
    statsRow->setSpacing(12);
    lblStatSolic = makeStat(pg,"0","SOLICITAÇÕES\nPENDENTES","#1B5E35");
    lblStatAtend = makeStat(pg,"0","ATENDIMENTOS\nHOJE",     "#1565C0");
    lblStatFila  = makeStat(pg,"0","EM ESPERA\nNA FILA",    "#B45309");
    lblStatTotal = makeStat(pg,"0","ATENDIMENTOS\nREALIZADOS","#374151");
    for (auto* l : {lblStatSolic,lblStatAtend,lblStatFila,lblStatTotal})
        statsRow->addWidget(l->parentWidget());

    // Linha inferior
    auto* botRow = new QHBoxLayout();
    botRow->setSpacing(14);

    // Card fila por prioridade
    auto* cardFila = makeCard(pg, "Fila por prioridade");
    auto* filaLay  = qobject_cast<QVBoxLayout*>(cardFila->layout());

    auto mkFilaRow = [&](const QString& cor, const QString& label, QLabel*& lbl){
        auto* row = new QHBoxLayout();
        auto* dot = new QLabel("●", cardFila);
        dot->setStyleSheet("color:" + cor + "; font-size:16px;");
        auto* lbL = new QLabel(label, cardFila);
        lbL->setStyleSheet("color:#111827; font-size:15px; font-weight:700;");
        lbl = new QLabel("0 pessoas", cardFila);
        lbl->setStyleSheet("color:#111827; font-size:14px; font-weight:700;");
        row->addWidget(dot); row->addWidget(lbL); row->addStretch(); row->addWidget(lbl);
        filaLay->addLayout(row);
    };
    mkFilaRow("#DC2626","1 - Alta",       lblDashAlta);
    mkFilaRow("#D97706","2 - Média",      lblDashMedia);
    mkFilaRow("#1D4ED8","3 - Baixa",      lblDashBaixa);
    mkFilaRow("#374151","4 - Muito baixa",lblDashMuito);

    // Card próximo
    auto* cardProx = makeCard(pg, "Próximo a ser atendido");
    auto* proxLay  = qobject_cast<QVBoxLayout*>(cardProx->layout());
    lblProxNome = new QLabel("–", cardProx);
    lblProxNome->setStyleSheet("font-size:18px; font-weight:800; color:#14532D;");
    lblProxPri  = new QLabel("–", cardProx);
    lblProxPri->setStyleSheet("font-size:14px; color:#111827; font-weight:600;");
    auto* btnChamar = new QPushButton("📢  Chamar próximo", cardProx);
    btnChamar->setObjectName("btnPrim");
    btnChamar->setMinimumHeight(42);
    btnChamar->setCursor(Qt::PointingHandCursor);
    proxLay->addWidget(lblProxNome);
    proxLay->addWidget(lblProxPri);
    proxLay->addStretch();
    proxLay->addWidget(btnChamar);

    botRow->addWidget(cardFila, 1);
    botRow->addWidget(cardProx, 1);

    lay->addWidget(hdr);
    lay->addWidget(sub);
    lay->addLayout(statsRow);
    lay->addLayout(botRow);
    lay->addStretch();

    connect(btnChamar, &QPushButton::clicked, this, &MenuFuncionarioWindow::onChamarProximo);
}

void MenuFuncionarioWindow::atualizarDashboard() {
    int sol = 0;
    for (auto u : usuarios) {
        Paciente* p = dynamic_cast<Paciente*>(u);
        if (p && p->possuiSolicitacao()) sol++;
    }
    if (lblStatSolic) lblStatSolic->setText(QString::number(sol));
    if (lblStatFila)  lblStatFila->setText(QString::number(fila.total()));
    if (lblStatAtend) lblStatAtend->setText(QString::number(atendimentosHoje)); // correção para atualizacao do dashboard
    if (lblStatTotal) lblStatTotal->setText(QString::number(atendimentosRealizados));
    if (lblDashAlta)  lblDashAlta->setText(QString::number(fila.totalAlta()) + " pessoas");
    if (lblDashMedia) lblDashMedia->setText(QString::number(fila.totalMedia()) + " pessoas");
    if (lblDashBaixa) lblDashBaixa->setText(QString::number(fila.totalBaixa()) + " pessoas");
    if (lblDashMuito) lblDashMuito->setText(QString::number(fila.totalMuitoBaixa()) + " pessoas");

    auto rank = fila.getRankFila();
    if (!rank.empty()) {
        auto& [p, pri] = rank[0];
        if (lblProxNome) lblProxNome->setText(QString::fromStdString(p->getNome()));
        QString priStr = (pri==1?"🔴 Alta":pri==2?"🟡 Média":pri==3?"🔵 Baixa":"⚫ Muito baixa");
        if (lblProxPri)  lblProxPri->setText("Prioridade: " + priStr);
    } else {
        if (lblProxNome) lblProxNome->setText("Fila vazia");
        if (lblProxPri)  lblProxPri->setText("–");
    }
}

// ── Solicitações ──────────────────────────────────────────────────────────────

void MenuFuncionarioWindow::buildPageSolicitacoes(QWidget* pg) {
    auto* lay = new QVBoxLayout(pg);
    lay->setContentsMargins(28,24,28,24);
    lay->setSpacing(14);

    auto* hdr = new QLabel("Solicitações pendentes", pg);
    hdr->setObjectName("lblPgTit");
    auto* sub = new QLabel("Analise e gerencie as solicitações de atendimento.", pg);
    sub->setObjectName("lblPgSub");

    lstSolic = new QListWidget(pg);
    lstSolic->setMinimumHeight(180);

    auto* lblSint = new QLabel("Sintomas / detalhes do paciente:", pg);
    lblSint->setObjectName("lblFieldLabel");
    txtSolicDetalhe = new QTextEdit(pg);
    txtSolicDetalhe->setReadOnly(true);
    txtSolicDetalhe->setMaximumHeight(110);
    txtSolicDetalhe->setObjectName("txtAreaInfo");
    txtSolicDetalhe->setPlaceholderText("Selecione um paciente para ver os sintomas descritos...");

    auto* formRow = new QHBoxLayout();
    auto* lblTipo = new QLabel("Tipo:", pg);
    lblTipo->setObjectName("lblFieldLabel");
    edtTipoSolic = new QLineEdit(pg);
    edtTipoSolic->setPlaceholderText("ex: Consulta Médica");
    edtTipoSolic->setMinimumHeight(38);
    auto* lblPri = new QLabel("Prioridade:", pg);
    lblPri->setObjectName("lblFieldLabel");
    cboPriSolic = new QComboBox(pg);
    cboPriSolic->addItems({"1 – Alta","2 – Média","3 – Baixa"});
    cboPriSolic->setMinimumHeight(38);
    formRow->addWidget(lblTipo);
    formRow->addWidget(edtTipoSolic, 2);
    formRow->addWidget(lblPri);
    formRow->addWidget(cboPriSolic, 1);

    auto* btnRow = new QHBoxLayout();
    auto* btnNegar   = new QPushButton("✗  Negar",  pg);
    btnNegar->setObjectName("btnNegar");
    btnNegar->setMinimumHeight(40);
    auto* btnAceitar = new QPushButton("✓  Aceitar e adicionar à fila", pg);
    btnAceitar->setObjectName("btnPrim");
    btnAceitar->setMinimumHeight(40);
    btnRow->addWidget(btnNegar);
    btnRow->addWidget(btnAceitar);

    lay->addWidget(hdr);
    lay->addWidget(sub);
    lay->addWidget(lstSolic);
    lay->addWidget(lblSint);
    lay->addWidget(txtSolicDetalhe);
    lay->addLayout(formRow);
    lay->addLayout(btnRow);
    lay->addStretch();

    connect(lstSolic,   &QListWidget::currentRowChanged, this, &MenuFuncionarioWindow::onSolicSelecionar);
    connect(btnAceitar, &QPushButton::clicked, this, &MenuFuncionarioWindow::onSolicAceitar);
    connect(btnNegar,   &QPushButton::clicked, this, &MenuFuncionarioWindow::onSolicNegar);
    atualizarSolicitacoes();
}

void MenuFuncionarioWindow::atualizarSolicitacoes() {
    if (!lstSolic) return;
    lstSolic->clear();
    pacientesComSol.clear();
    for (auto u : usuarios) {
        Paciente* p = dynamic_cast<Paciente*>(u);
        if (p && p->possuiSolicitacao()) {
            pacientesComSol.push_back(p);
            lstSolic->addItem("  " + QString::fromStdString(p->getNome()) +
                              "  [" + QString::fromStdString(p->getLogin()) + "]");
        }
    }
    if (pacientesComSol.empty() && txtSolicDetalhe)
        txtSolicDetalhe->setText("Nenhuma solicitação pendente no momento.");
}

void MenuFuncionarioWindow::onSolicSelecionar(int row) {
    if (row < 0 || row >= (int)pacientesComSol.size() || !txtSolicDetalhe) return;
    Paciente* p = pacientesComSol[row];
    txtSolicDetalhe->setText(
        "Paciente: " + QString::fromStdString(p->getNome()) + "\n"
                                                              "Status: "   + QString::fromStdString(p->getStatus()) + "\n\n"
                                                   "Sintomas descritos pelo paciente:\n" +
        QString::fromStdString(p->getDescricaoSolicitacao())
        );
}

void MenuFuncionarioWindow::onSolicAceitar() {
    int row = lstSolic->currentRow();
    if (row < 0 || row >= (int)pacientesComSol.size()) {
        QMessageBox::warning(this,"Atenção","Selecione um paciente."); return;
    }
    std::string tipo = edtTipoSolic->text().trimmed().toStdString();
    if (tipo.empty()) { QMessageBox::warning(this,"Atenção","Informe o tipo de atendimento."); return; }
    Paciente* p = pacientesComSol[row];
    int pri = cboPriSolic->currentIndex() + 1;
    auto* t = new Triagem(p->getDescricaoSolicitacao(), tipo, pri);
    p->setTriagem(t);
    fila.adicionarPaciente(p);
    p->adicionarHistorico("Solicitação ACEITA | Tipo: " + tipo + " | Prioridade: " + std::to_string(pri));
    p->limparSolicitacao();
    QMessageBox::information(this,"Sucesso","Paciente adicionado à fila!");
    atualizarSolicitacoes();
    atualizarDashboard();
}

void MenuFuncionarioWindow::onSolicNegar() {
    int row = lstSolic->currentRow();
    if (row < 0 || row >= (int)pacientesComSol.size()) {
        QMessageBox::warning(this,"Atenção","Selecione um paciente."); return;
    }
    Paciente* p = pacientesComSol[row];
    p->adicionarHistorico("Solicitação NEGADA: " + p->getDescricaoSolicitacao());
    p->limparSolicitacao();
    QMessageBox::information(this,"Negado","Solicitação negada.");
    atualizarSolicitacoes();
    atualizarDashboard();
}

// ── Triagem ───────────────────────────────────────────────────────────────────

void MenuFuncionarioWindow::buildPageTriagem(QWidget* pg) {
    auto* lay = new QVBoxLayout(pg);
    lay->setContentsMargins(28,24,28,24);
    lay->setSpacing(14);

    auto* hdr = new QLabel("Triagem", pg);
    hdr->setObjectName("lblPgTit");
    auto* sub = new QLabel("Realize a triagem do paciente.", pg);
    sub->setObjectName("lblPgSub");

    auto* twoCol = new QHBoxLayout();
    twoCol->setSpacing(14);

    // ── Coluna esquerda: dados do paciente ──
    auto* cardEsq = makeCard(pg, "Dados do paciente");
    auto* esqLay  = qobject_cast<QVBoxLayout*>(cardEsq->layout());

    edtTriagemLogin = new QLineEdit(cardEsq);
    edtTriagemLogin->setPlaceholderText("Login do paciente");
    edtTriagemLogin->setMinimumHeight(38);

    auto* btnBuscar = new QPushButton("🔍  Buscar", cardEsq);
    btnBuscar->setObjectName("btnSec");
    btnBuscar->setMinimumHeight(36);

    lblTriagemNome  = new QLabel("Nome: –",  cardEsq);
    lblTriagemCurso = new QLabel("Curso: –", cardEsq);
    lblTriagemIdade = new QLabel("Idade: –", cardEsq);
    for (auto* l : {lblTriagemNome, lblTriagemCurso, lblTriagemIdade})
        l->setStyleSheet("color:#111827; font-size:13px; font-weight:500; padding:4px 0;");

    auto* lblSint = new QLabel("Solicitação do paciente:", cardEsq);
    lblSint->setStyleSheet("font-weight:700; color:#1B5E35; font-size:13px; margin-top:6px;");
    txtTriagemSint = new QTextEdit(cardEsq);
    txtTriagemSint->setMaximumHeight(90);
    txtTriagemSint->setObjectName("txtAreaInfo");
    txtTriagemSint->setPlaceholderText("Sintomas descritos pelo paciente...");

    esqLay->addWidget(edtTriagemLogin);
    esqLay->addWidget(btnBuscar);
    esqLay->addSpacing(8);
    esqLay->addWidget(lblTriagemNome);
    esqLay->addWidget(lblTriagemCurso);
    esqLay->addWidget(lblTriagemIdade);
    esqLay->addWidget(lblSint);
    esqLay->addWidget(txtTriagemSint);
    esqLay->addStretch();

    // ── Coluna direita: tipo + prioridade ──
    auto* cardDir = makeCard(pg, "Tipo de atendimento");
    auto* dirLay  = qobject_cast<QVBoxLayout*>(cardDir->layout());

    cboTriagemTipo = new QComboBox(cardDir);
    cboTriagemTipo->addItems({
        "Médico Clínico Geral",
        "Odontológico",
        "Psicológico",
        "Nutricional",
        "Enfermagem",
        "Outros"
    });
    cboTriagemTipo->setMinimumHeight(38);

    auto* lblPriTit = new QLabel("Prioridade", cardDir);
    lblPriTit->setStyleSheet("font-weight:700; color:#111827; font-size:14px; margin-top:8px;");

    auto mkPriBtn = [&](const QString& cor, const QString& txt, int val) {
        auto* b = new QPushButton("● " + txt, cardDir);
        b->setCheckable(true);
        b->setObjectName("btnPriOption");
        b->setProperty("priVal", val);
        b->setMinimumHeight(38);
        b->setCursor(Qt::PointingHandCursor);
        b->setStyleSheet(
            "QPushButton { background:white; border:1.5px solid #9CA3AF; border-radius:6px;"
            "  color:#111827; padding:6px 12px; text-align:left; font-size:13px; font-weight:500; }"
            "QPushButton:checked { background:" + cor + "; color:white; border-color:" + cor + "; font-weight:700; }"
                                                           "QPushButton:hover:!checked { border-color:" + cor + "; color:" + cor + "; }"
            );
        return b;
    };
    btnPri1 = mkPriBtn("#DC2626","1 - Alta",1);
    btnPri2 = mkPriBtn("#D97706","2 - Média",2);
    btnPri3 = mkPriBtn("#1D4ED8","3 - Baixa",3);
    btnPri4 = mkPriBtn("#374151","4 - Muito baixa (não bolsista)",4);
    btnPri2->setChecked(true);

    auto checkPri = [=](QPushButton* active){
        for (auto* b : std::vector<QPushButton*>{btnPri1,btnPri2,btnPri3,btnPri4})
            b->setChecked(b==active);
    };
    connect(btnPri1,&QPushButton::clicked,this,[=](){checkPri(btnPri1);});
    connect(btnPri2,&QPushButton::clicked,this,[=](){checkPri(btnPri2);});
    connect(btnPri3,&QPushButton::clicked,this,[=](){checkPri(btnPri3);});
    connect(btnPri4,&QPushButton::clicked,this,[=](){checkPri(btnPri4);});

    dirLay->addWidget(cboTriagemTipo);
    dirLay->addWidget(lblPriTit);
    dirLay->addWidget(btnPri1);
    dirLay->addWidget(btnPri2);
    dirLay->addWidget(btnPri3);
    dirLay->addWidget(btnPri4);
    dirLay->addStretch();

    twoCol->addWidget(cardEsq, 1);
    twoCol->addWidget(cardDir, 1);

    lblTriagemErro = new QLabel("", pg);
    lblTriagemErro->setObjectName("lblErro");
    lblTriagemErro->hide();

    auto* btnConfirmar = new QPushButton("✓  Confirmar triagem", pg);
    btnConfirmar->setObjectName("btnPrim");
    btnConfirmar->setMinimumHeight(46);
    btnConfirmar->setCursor(Qt::PointingHandCursor);

    lay->addWidget(hdr);
    lay->addWidget(sub);
    lay->addLayout(twoCol);
    lay->addWidget(lblTriagemErro);
    lay->addWidget(btnConfirmar);
    lay->addStretch();

    connect(btnBuscar,    &QPushButton::clicked, this, &MenuFuncionarioWindow::onTriagemBuscar);
    connect(btnConfirmar, &QPushButton::clicked, this, &MenuFuncionarioWindow::onTriagemConfirmar);
}

void MenuFuncionarioWindow::onTriagemBuscar() {
    lblTriagemErro->hide();
    std::string login = edtTriagemLogin->text().trimmed().toStdString();
    pacienteTriagem = nullptr;
    for (auto u : usuarios) {
        Paciente* p = dynamic_cast<Paciente*>(u);
        if (p && p->getLogin() == login) {
            pacienteTriagem = p;
            lblTriagemNome->setText("Nome: "  + QString::fromStdString(p->getNome()));
            lblTriagemIdade->setText("Idade: " + QString::number(p->getIdade()));
            lblTriagemCurso->setText("Curso: –");
            if (p->possuiSolicitacao())
                txtTriagemSint->setText(QString::fromStdString(p->getDescricaoSolicitacao()));
            return;
        }
    }
    lblTriagemErro->setText("Paciente não encontrado."); lblTriagemErro->show();
}

void MenuFuncionarioWindow::onTriagemConfirmar() {
    lblTriagemErro->hide();
    if (!pacienteTriagem) {
        lblTriagemErro->setText("Busque o paciente primeiro."); lblTriagemErro->show(); return;
    }
    if (pacienteTriagem->fezTriagem()) {
        lblTriagemErro->setText("Paciente já passou pela triagem inicial."); lblTriagemErro->show(); return;
    }
    std::string sint = txtTriagemSint->toPlainText().trimmed().toStdString();
    if (sint.empty()) {
        lblTriagemErro->setText("Informe os sintomas."); lblTriagemErro->show(); return;
    }
    int pri = 2;
    for (auto* b : std::vector<QPushButton*>{btnPri1,btnPri2,btnPri3,btnPri4})
        if (b->isChecked()) { pri = b->property("priVal").toInt(); break; }

    std::string tipo = cboTriagemTipo->currentText().toStdString();
    auto* t = new Triagem(sint, tipo, pri);
    pacienteTriagem->setTriagem(t);
    fila.adicionarPaciente(pacienteTriagem);
    pacienteTriagem->adicionarHistorico("Triagem | Tipo: " + tipo + " | Prioridade: " + std::to_string(pri));
    pacienteTriagem->marcarTriagemRealizada();

    pacienteTriagem->limparSolicitacao();

    pacienteTriagem->updateInDatabase();
    QMessageBox::information(this,"Sucesso","Triagem realizada e paciente adicionado à fila!");
    atualizarDashboard();
    edtTriagemLogin->clear(); txtTriagemSint->clear();
    lblTriagemNome->setText("Nome: –"); lblTriagemCurso->setText("Curso: –");
    lblTriagemIdade->setText("Idade: –"); pacienteTriagem = nullptr;
}

// ── Fila ──────────────────────────────────────────────────────────────────────

void MenuFuncionarioWindow::buildPageFila(QWidget* pg) {
    auto* lay = new QVBoxLayout(pg);
    lay->setContentsMargins(28,24,28,24);
    lay->setSpacing(14);

    auto* hdr = new QLabel("Filas / Ordem de atendimento", pg);
    hdr->setObjectName("lblPgTit");
    auto* sub = new QLabel("Confira a ordem atual das filas por prioridade.", pg);
    sub->setObjectName("lblPgSub");

    auto* colRow = new QHBoxLayout();
    colRow->setSpacing(10);

    auto mkFilaCol = [&](const QString& cor, const QString& titulo, QListWidget*& lst) -> QWidget* {
        auto* card = new QWidget(pg);
        card->setObjectName("card");
        auto* cl = new QVBoxLayout(card);
        cl->setContentsMargins(12,14,12,14);
        cl->setSpacing(6);
        auto* lbl = new QLabel(titulo, card);
        lbl->setStyleSheet("font-weight:800; font-size:14px; color:" + cor + ";");
        lst = new QListWidget(card);
        lst->setStyleSheet(
            "QListWidget { border:none; background:transparent; }"
            "QListWidget::item { background:#F9FAFB; border:1px solid #E5E7EB; border-radius:5px;"
            "  margin:2px 0; padding:7px 9px; color:#111827; font-size:12px; font-weight:500; }"
            "QListWidget::item:selected { background:" + cor + "; color:white; }"
            );
        auto* lblTotal = new QLabel("Total: 0", card);
        lblTotal->setObjectName("lblFilaTotal");
        lblTotal->setStyleSheet("color:#374151; font-size:12px; font-weight:600;");
        cl->addWidget(lbl);
        cl->addWidget(lst, 1);
        cl->addWidget(lblTotal);
        return card;
    };

    colRow->addWidget(mkFilaCol("#DC2626","1 - Alta",       lstFilaAlta));
    colRow->addWidget(mkFilaCol("#D97706","2 - Média",      lstFilaMedia));
    colRow->addWidget(mkFilaCol("#1D4ED8","3 - Baixa",      lstFilaBaixa));
    colRow->addWidget(mkFilaCol("#374151","4 - Muito baixa",lstFilaMuito));

    auto* btnAtualizar = new QPushButton("🔄  Atualizar filas", pg);
    btnAtualizar->setObjectName("btnSec");
    btnAtualizar->setMinimumHeight(38);
    btnAtualizar->setMaximumWidth(200);
    btnAtualizar->setCursor(Qt::PointingHandCursor);

    lay->addWidget(hdr);
    lay->addWidget(sub);
    lay->addLayout(colRow);
    lay->addWidget(btnAtualizar);
    lay->addStretch();

    connect(btnAtualizar, &QPushButton::clicked, this, &MenuFuncionarioWindow::atualizarFila);
}

void MenuFuncionarioWindow::atualizarFila() {
    if (!lstFilaAlta) return;
    for (auto* lst : {lstFilaAlta, lstFilaMedia, lstFilaBaixa, lstFilaMuito}) lst->clear();

    auto rank = fila.getRankFila();
    int ca=0, cm=0, cb=0, cmb=0, pos=1;
    for (auto& [p, pri] : rank) {
        QString item = QString("%1. %2").arg(pos).arg(QString::fromStdString(p->getNome()));
        if      (pri==1){ lstFilaAlta->addItem(item);  ca++; }
        else if (pri==2){ lstFilaMedia->addItem(item); cm++; }
        else if (pri==3){ lstFilaBaixa->addItem(item); cb++; }
        else            { lstFilaMuito->addItem(item); cmb++; }
        pos++;
    }
    auto setTotal = [](QListWidget* lst, int total) {
        if (!lst || !lst->parentWidget()) return;
        auto* lbl = lst->parentWidget()->findChild<QLabel*>("lblFilaTotal");
        if (lbl) lbl->setText("Total: " + QString::number(total));
    };
    setTotal(lstFilaAlta, ca); setTotal(lstFilaMedia, cm);
    setTotal(lstFilaBaixa, cb); setTotal(lstFilaMuito, cmb);
}

// ── Cadastro ──────────────────────────────────────────────────────────────────

void MenuFuncionarioWindow::buildPageCadastro(QWidget* pg) {
    auto* lay = new QVBoxLayout(pg);
    lay->setContentsMargins(28,24,28,24);
    lay->setSpacing(12);
    auto* hdr = new QLabel("Cadastrar aluno", pg);
    hdr->setObjectName("lblPgTit");
    auto* sub = new QLabel("Preencha os dados para cadastrar um novo paciente.", pg);
    sub->setObjectName("lblPgSub");
    auto* btnAbrir = new QPushButton("  Abrir formulário de cadastro", pg);
    btnAbrir->setObjectName("btnPrim");
    btnAbrir->setMinimumHeight(46);
    btnAbrir->setMaximumWidth(300);
    btnAbrir->setCursor(Qt::PointingHandCursor);
    lay->addWidget(hdr); lay->addWidget(sub);
    lay->addSpacing(20); lay->addWidget(btnAbrir); lay->addStretch();
    connect(btnAbrir, &QPushButton::clicked, this, [=](){
        CadastroDialog dlg(usuarios, db, this);
        dlg.exec(); atualizarDashboard();
    });
}

// ── Histórico ─────────────────────────────────────────────────────────────────

void MenuFuncionarioWindow::buildPageHistorico(QWidget* pg) {
    auto* lay = new QVBoxLayout(pg);
    lay->setContentsMargins(28,24,28,24);
    lay->setSpacing(12);
    auto* hdr = new QLabel("Histórico de atendimentos", pg);
    hdr->setObjectName("lblPgTit");
    auto* sub = new QLabel("Acompanhe o histórico de solicitações e atendimentos.", pg);
    sub->setObjectName("lblPgSub");
    txtHistorico = new QTextEdit(pg);
    txtHistorico->setReadOnly(true);
    txtHistorico->setObjectName("txtAreaInfo");
    auto* btnAt = new QPushButton("🔄  Atualizar", pg);
    btnAt->setObjectName("btnSec");
    btnAt->setMinimumHeight(36);
    btnAt->setMaximumWidth(180);
    lay->addWidget(hdr); lay->addWidget(sub);
    lay->addWidget(btnAt); lay->addWidget(txtHistorico);
    connect(btnAt, &QPushButton::clicked, this, &MenuFuncionarioWindow::atualizarHistorico);
}

void MenuFuncionarioWindow::atualizarHistorico() {
    if (!txtHistorico) return;
    txtHistorico->clear();
    bool encontrou = false;
    for (auto u : usuarios) {
        Paciente* p = dynamic_cast<Paciente*>(u);
        if (p) {
            encontrou = true;
            txtHistorico->append("▸ " + QString::fromStdString(p->getNome()) +
                                 "  [" + QString::fromStdString(p->getLogin()) + "]  –  " +
                                 QString::fromStdString(p->getStatus()));
            auto hist = p->getHistoricoSolicitacoes();
            if (hist.empty()) txtHistorico->append("  (sem registros)\n");
            else { for (const auto& h : hist) txtHistorico->append("  • " + QString::fromStdString(h)); txtHistorico->append(""); }
        }
    }
    if (!encontrou) txtHistorico->setText("Nenhum paciente cadastrado.");
}

// ── Editar ────────────────────────────────────────────────────────────────────

void MenuFuncionarioWindow::buildPageEditar(QWidget* pg) {
    auto* lay = new QVBoxLayout(pg);
    lay->setContentsMargins(28,24,28,24);
    lay->setSpacing(12);
    auto* hdr = new QLabel("Editar dados de paciente", pg);
    hdr->setObjectName("lblPgTit");
    auto* sub = new QLabel("Selecione o paciente e edite suas informações.", pg);
    sub->setObjectName("lblPgSub");
    auto* btnAbrir = new QPushButton("  Selecionar paciente e editar", pg);
    btnAbrir->setObjectName("btnPrim");
    btnAbrir->setMinimumHeight(46);
    btnAbrir->setMaximumWidth(300);
    btnAbrir->setCursor(Qt::PointingHandCursor);
    lay->addWidget(hdr); lay->addWidget(sub);
    lay->addSpacing(20); lay->addWidget(btnAbrir); lay->addStretch();
    connect(btnAbrir, &QPushButton::clicked, this, &MenuFuncionarioWindow::onEditarDados);
}

// ── Ações gerais ──────────────────────────────────────────────────────────────

void MenuFuncionarioWindow::onChamarProximo() {
    Paciente* p = fila.chamarProximo();

    if (p) {
        p->setEmFila(false);
        p->finalizarAtendimento();

        // Atualiza contadores primeiro
        atendimentosHoje++;
        atendimentosRealizados++;

        // Atualiza dashboard depois
        atualizarDashboard();
        atualizarFila();

        QMessageBox::information(
            this,
            "Paciente chamado",
            "📢  " + QString::fromStdString(p->getNome()) +
                " foi chamado para atendimento!"
            );

    } else {
        QMessageBox::information(
            this,
            "Fila vazia",
            "Nenhum paciente aguardando."
            );
    }
}

void MenuFuncionarioWindow::onEditarDados() {
    QStringList nomes;
    std::vector<Paciente*> pacs;
    for (auto u : usuarios) {
        Paciente* p = dynamic_cast<Paciente*>(u);
        if (p) { pacs.push_back(p); nomes << QString::fromStdString(p->getNome()) + " [" + QString::fromStdString(p->getLogin()) + "]"; }
    }
    if (pacs.empty()) { QMessageBox::information(this,"Info","Nenhum paciente cadastrado."); return; }
    bool ok;
    QString escolha = QInputDialog::getItem(this,"Editar Dados","Selecione o paciente:",nomes,0,false,&ok);
    if (!ok) return;
    int idx = nomes.indexOf(escolha);
    if (idx < 0) return;
    EditarDadosDialog dlg(pacs[idx], usuarios, this);
    dlg.exec();
}

void MenuFuncionarioWindow::onVoltar() { close(); }

// ── Estilo ────────────────────────────────────────────────────────────────────

void MenuFuncionarioWindow::applyStyle() {
    setStyleSheet(QString(NASE_GLOBAL_STYLE) + R"(

        /* Fundo geral */
        QWidget { background-color: #F3F8F5; }
        /* Stat cards — fundo colorido vem do setStyleSheet inline, preservar */
        QWidget#statCard { border-radius: 10px; }

        /* ── Sidebar ── */
        QWidget#sidebar { background-color: #14532D; }
        QPushButton#btnMenu {
            background-color: transparent;
            color: rgba(255,255,255,0.80);
            border: none; border-radius: 7px;
            text-align: left; padding: 11px 16px; font-size: 14px;
        }
        QPushButton#btnMenu:hover   { background-color: rgba(255,255,255,0.14); color: white; }
        QPushButton#btnMenu:checked {
            background-color: rgba(255,255,255,0.22);
            color: white; font-weight: bold;
            border-left: 3px solid #4ade80;
        }
        QPushButton#btnSideExit {
            background: transparent; color: rgba(255,255,255,0.55);
            border: 1px solid rgba(255,255,255,0.25); border-radius: 7px;
            text-align: left; padding: 11px 16px; font-size: 14px;
        }
        QPushButton#btnSideExit:hover { color: white; border-color: rgba(255,255,255,0.6); }
        QLabel#lblSideTitle { color: #86efac; font-size: 11px; font-weight: bold; letter-spacing: 2px; }
        QLabel#lblSideUser  { color: rgba(255,255,255,0.65); font-size: 12px; }

        /* ── Títulos de página ── */
        QLabel#lblPgTit { font-size: 24px; font-weight: 800; color: #14532D; }
        QLabel#lblPgSub { font-size: 13px; color: #374151; font-weight: 500; }

        /* ── Labels de campo ── */
        QLabel#lblFieldLabel { font-size: 13px; font-weight: 700; color: #111827; }

        /* ── Cards ── */
        QWidget#card {
            background: white; border-radius: 12px;
            border: 1.5px solid #D1FAE5;
        }
        QLabel#lblCardTit { font-size: 15px; font-weight: 800; color: #14532D; }

        /* ── Área de texto ── */
        QTextEdit#txtAreaInfo {
            background: #F9FAFB; border: 1.5px solid #D1D5DB;
            border-radius: 8px; color: #111827; font-size: 13px;
            font-weight: 500; padding: 10px;
        }

        /* ── Inputs ── */
        QLineEdit, QComboBox {
            background: white; border: 1.5px solid #D1D5DB;
            border-radius: 7px; color: #111827; font-size: 13px;
            font-weight: 500; padding: 5px 12px;
        }
        QLineEdit:focus, QComboBox:focus { border-color: #1B5E35; }

        /* ── ListWidget ── */
        QListWidget {
            background: white; border: 1.5px solid #D1D5DB;
            border-radius: 8px; color: #111827;
        }
        QListWidget::item { padding:10px 14px; color:#111827; font-size:13px; font-weight:500; }
        QListWidget::item:selected { background: #1B5E35; color: white; }
        QListWidget::item:hover    { background: #DCFCE7; }

        /* ── Botões ── */
        QPushButton#btnPrim {
            background-color: #15803D; color: white; border: none;
            border-radius: 7px; font-weight: 700; font-size: 14px;
            padding: 8px 20px; min-height: 36px;
        }
        QPushButton#btnPrim:hover { background-color: #166534; }
        QPushButton#btnSec {
            background-color: white; color: #15803D;
            border: 1.5px solid #15803D; border-radius: 7px;
            font-weight: 700; font-size: 14px; padding: 8px 20px; min-height: 36px;
        }
        QPushButton#btnSec:hover { background-color: #DCFCE7; }
        QPushButton#btnNegar {
            background-color: #DC2626; color: white; border: none;
            border-radius: 7px; font-weight: 700; font-size: 14px;
            padding: 8px 20px; min-height: 36px;
        }
        QPushButton#btnNegar:hover { background-color: #B91C1C; }
        QPushButton#btnVoltar {
            background-color: transparent; color: #6B7280;
            border: 1.5px solid #D1D5DB; border-radius: 7px;
            font-size: 13px; padding: 8px;
        }
        QPushButton#btnVoltar:hover { color: #DC2626; border-color: #DC2626; }
    )");
}
