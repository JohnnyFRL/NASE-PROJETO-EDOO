#include "MenuPacienteWindow.h"
#include "NaseStyle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QInputDialog>
#include <QMessageBox>

MenuPacienteWindow::MenuPacienteWindow(Paciente* paciente, std::vector<Usuario*>& usuarios,
                                       FilaPrioridade& fila, Database* db, QWidget* parent)
    : QWidget(parent), paciente(paciente), usuarios(usuarios), fila(fila), db(db) {
    setWindowTitle("Portal do Paciente – NASE UFPE");
    setMinimumSize(700, 560);
    resize(720, 580);

    auto* root = new QHBoxLayout(this);
    root->setContentsMargins(0,0,0,0);
    root->setSpacing(0);

    // ── Sidebar ──
    auto* sidebar = new QWidget(this);
    sidebar->setFixedWidth(200);
    sidebar->setObjectName("sidebar");
    auto* sideL = new QVBoxLayout(sidebar);
    sideL->setContentsMargins(12,20,12,20);
    sideL->setSpacing(4);

    auto* lblIco   = new QLabel("👤", sidebar);
    lblIco->setAlignment(Qt::AlignCenter);
    lblIco->setStyleSheet("font-size:32px; color:white; margin-bottom:4px;");
    auto* lblRole  = new QLabel("PACIENTE", sidebar);
    lblRole->setObjectName("lblSideTitle");
    lblRole->setAlignment(Qt::AlignCenter);
    auto* lblUser  = new QLabel(QString::fromStdString(paciente->getNome()).split(" ").first(), sidebar);
    lblUser->setObjectName("lblSideUser");
    lblUser->setAlignment(Qt::AlignCenter);

    auto* sep = new QFrame(sidebar);
    sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color:rgba(255,255,255,0.2);");

    auto mkSideBtn = [&](const QString& ico, const QString& txt) {
        auto* b = new QPushButton(ico + "  " + txt, sidebar);
        b->setObjectName("btnMenu");
        b->setCheckable(true);
        b->setMinimumHeight(40);
        b->setCursor(Qt::PointingHandCursor);
        return b;
    };
    auto* btnSolicitar = mkSideBtn("📋","Solicitar consulta");
    auto* btnHistorico = mkSideBtn("🕐","Ver histórico");
    auto* btnDados     = mkSideBtn("👤","Meus dados");
    auto* btnSair      = new QPushButton("  ← Sair", sidebar);
    btnSair->setObjectName("btnSideExit");
    btnSair->setMinimumHeight(40);
    btnSair->setCursor(Qt::PointingHandCursor);

    sideL->addWidget(lblIco);
    sideL->addWidget(lblRole);
    sideL->addWidget(lblUser);
    sideL->addWidget(sep);
    sideL->addSpacing(8);
    sideL->addWidget(btnSolicitar);
    sideL->addWidget(btnHistorico);
    sideL->addWidget(btnDados);
    sideL->addStretch();
    sideL->addWidget(btnSair);

    // ── Conteúdo ──
    auto* content  = new QWidget(this);
    auto* contentL = new QVBoxLayout(content);
    contentL->setContentsMargins(28,24,28,24);
    contentL->setSpacing(16);

    auto* hdrRow = new QHBoxLayout();
    lblTitulo = new QLabel("Solicitar consulta", content);
    lblTitulo->setObjectName("lblConteudo");
    lblStatus = new QLabel("", content);
    lblStatus->setObjectName("lblStatus");
    atualizarStatus();
    hdrRow->addWidget(lblTitulo);
    hdrRow->addStretch();
    hdrRow->addWidget(lblStatus);

    auto* card = new QWidget(content);
    card->setObjectName("card");
    auto* cardL = new QVBoxLayout(card);
    cardL->setContentsMargins(20,20,20,20);
    cardL->setSpacing(12);

    txtInfo = new QTextEdit(card);
    txtInfo->setReadOnly(true);
    txtInfo->setMinimumHeight(200);
    txtInfo->setObjectName("txtAreaInfo");
    txtInfo->setPlaceholderText("Descreva seus sintomas ou necessidade...");

    auto* lblInfoBar = new QLabel("ℹ️  Sua solicitação será analisada pela equipe de saúde.", card);
    lblInfoBar->setObjectName("lblInfoBar");
    lblInfoBar->setWordWrap(true);

    auto* btnEnviar = new QPushButton("  Enviar solicitação", card);
    btnEnviar->setObjectName("btnPrim");
    btnEnviar->setMinimumHeight(42);
    btnEnviar->setMinimumWidth(200);
    btnEnviar->setCursor(Qt::PointingHandCursor);
    auto* btnRow = new QHBoxLayout();
    btnRow->addStretch();
    btnRow->addWidget(btnEnviar);

    cardL->addWidget(txtInfo);
    cardL->addWidget(lblInfoBar);
    cardL->addLayout(btnRow);

    contentL->addLayout(hdrRow);
    contentL->addWidget(card);
    contentL->addStretch();

    root->addWidget(sidebar);
    root->addWidget(content);

    auto checkExclusive = [=](QPushButton* active){
        for (auto* b : std::vector<QPushButton*>{btnSolicitar, btnHistorico, btnDados})
            b->setChecked(b == active);
    };

    connect(btnSolicitar, &QPushButton::clicked, this, [=](){
        checkExclusive(btnSolicitar);
        lblTitulo->setText("Solicitar consulta");
        onSolicitarConsulta();
    });
    connect(btnHistorico, &QPushButton::clicked, this, [=](){
        checkExclusive(btnHistorico);
        lblTitulo->setText("Histórico de atendimentos");
        onVerHistorico();
    });
    connect(btnDados, &QPushButton::clicked, this, [=](){
        checkExclusive(btnDados);
        lblTitulo->setText("Meus dados");
        onVerDados();
    });
    connect(btnEnviar, &QPushButton::clicked, this, &MenuPacienteWindow::onSolicitarConsulta);
    connect(btnSair,   &QPushButton::clicked, this, &MenuPacienteWindow::onVoltar);

    btnSolicitar->setChecked(true);
    applyStyle();
}

void MenuPacienteWindow::atualizarStatus() {
    QString s   = QString::fromStdString(paciente->getStatus());
    QString cor = "#DC2626";
    if (paciente->getStatusEnum() == VALIDADO)             cor = "#1B5E35";
    else if (paciente->getStatusEnum() == PERDE_PRIORIDADE) cor = "#d97706";
    lblStatus->setText("<span style='background:" + cor + ";color:white;padding:3px 10px;"
                       "border-radius:10px;font-size:11px;font-weight:bold'>" + s + "</span>");
    lblStatus->setTextFormat(Qt::RichText);
}

void MenuPacienteWindow::onSolicitarConsulta() {
    if (paciente->estaNaFila()) {
        txtInfo->setText("ℹ️  Você já está na fila de atendimento.\nAguarde ser chamado.");
        return;
    }
    // Usa fezTriagem() — método público que substitui acesso direto ao privado jaFezTriagem
    if (!paciente->fezTriagem()) {
        txtInfo->setText("⚠️  Você precisa passar pela triagem inicial primeiro.\n\nProcure um atendente no balcão.");
        return;
    }
    // Usa possuiSolicitacao() — método público que substitui temSolicitacao
    if (paciente->possuiSolicitacao()) {
        txtInfo->setText(
            "📋  Você já possui uma solicitação em andamento.\n\n"
            "Sintomas informados:\n" +
            QString::fromStdString(paciente->getDescricaoSolicitacao()) +
            "\n\nAguarde a avaliação do atendente.");
        return;
    }

    bool ok;
    QString sintomas = QInputDialog::getMultiLineText(
        this, "Solicitar Consulta", "Descreva seus sintomas detalhadamente:", "", &ok);

    if (ok && !sintomas.trimmed().isEmpty()) {
        // Usa criarSolicitacao() — método público que substitui acesso direto
        paciente->criarSolicitacao(sintomas.toStdString());
        paciente->adicionarHistorico("Solicitação enviada: " + sintomas.toStdString());
        paciente->updateInDatabase();

        txtInfo->setText("✅  Solicitação enviada!\n\nSintomas: " + sintomas +
                         "\n\nAguarde a avaliação do atendente.");
        QMessageBox::information(this, "Enviado", "Solicitação registrada!\nO atendente irá avaliar em breve.");
    }
}

void MenuPacienteWindow::onVerHistorico() {
    auto hist = paciente->getHistoricoSolicitacoes();
    if (hist.empty()) { txtInfo->setText("Nenhum registro encontrado."); return; }
    QString txt = "HISTÓRICO DE ATENDIMENTOS\n─────────────────────────────\n";
    for (const auto& h : hist)
        txt += "\n•  " + QString::fromStdString(h);
    txtInfo->setText(txt);
}

void MenuPacienteWindow::onVerDados() {
    txtInfo->setText(QString::fromStdString(paciente->getDadosBasicos()));
}

void MenuPacienteWindow::onVoltar() { close(); }

void MenuPacienteWindow::applyStyle() {
    setStyleSheet(QString(NASE_GLOBAL_STYLE) + R"(
        QWidget#sidebar { background-color: #1B5E35; }
        QPushButton#btnMenu {
            background-color:transparent; color:rgba(255,255,255,0.75);
            border:none; border-radius:6px; text-align:left; padding:10px 14px; font-size:13px;
        }
        QPushButton#btnMenu:hover   { background-color:rgba(255,255,255,0.12); color:white; }
        QPushButton#btnMenu:checked { background-color:rgba(255,255,255,0.20); color:white; font-weight:bold; border-left:3px solid #4ade80; }
        QPushButton#btnSideExit {
            background:transparent; color:rgba(255,255,255,0.5);
            border:1px solid rgba(255,255,255,0.2); border-radius:6px;
            text-align:left; padding:10px 14px; font-size:13px;
        }
        QPushButton#btnSideExit:hover { color:white; border-color:rgba(255,255,255,0.5); }
        QLabel#lblSideTitle { color:white; font-size:11px; font-weight:bold; letter-spacing:2px; }
        QLabel#lblSideUser  { color:rgba(255,255,255,0.6); font-size:11px; }
        QLabel#lblConteudo  { font-size:20px; font-weight:bold; color:#1B5E35; }
        QWidget#card { background:white; border-radius:10px; border:1px solid #C8DDD2; }
        QTextEdit#txtAreaInfo { background:#FAFFFE; border:1.5px solid #C8DDD2; border-radius:6px; color:#111827; font-size:13px; padding:10px; }
        QLabel#lblInfoBar { background:#E8F5EE; border:1px solid #C8DDD2; border-radius:6px; padding:8px 12px; color:#2E7D52; font-size:12px; }
    )");
}
