#include "MainWindow.h"
#include "MenuPacienteWindow.h"
#include "MenuFuncionarioWindow.h"
#include <QApplication>
#include <QPainter>
#include <QPixmap>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    db = new Database("sistema_medico.db");
    db->initialize();

    // Carregar pacientes do banco
    auto pacientes = Paciente::loadFromDatabase(db);
    for (auto p : pacientes)
        usuarios.push_back(p);

    // Admin padrão
    if (!db->userExists("admin"))
        db->saveUser("admin", "Admin@123", "funcionario");

    bool adminExists = false;
    for (auto u : usuarios)
        if (u->getLogin() == "admin") { adminExists = true; break; }
    if (!adminExists)
        usuarios.push_back(new Funcionario(db, "admin", "Admin@123"));

    setupUI();
}

MainWindow::~MainWindow() {
    for (auto u : usuarios) delete u;
    delete db;
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QMainWindow::paintEvent(event);
    QPainter painter(this);

    QPixmap bg(":/nase_bg.png");
    if (!bg.isNull()) {
        int panelW = (int)(width() * 0.42);
        QRect bgRect(panelW, 0, width() - panelW, height());
        QPixmap scaled = bg.scaled(bgRect.size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        int ox = (scaled.width()  - bgRect.width())  / 2;
        int oy = (scaled.height() - bgRect.height()) / 2;
        painter.drawPixmap(bgRect, scaled, QRect(ox, oy, bgRect.width(), bgRect.height()));
        QLinearGradient fadeL(panelW, 0, panelW + 120, 0);
        fadeL.setColorAt(0.0, QColor(0xEC, 0xF8, 0xF2, 255));
        fadeL.setColorAt(1.0, QColor(0xEC, 0xF8, 0xF2, 0));
        painter.fillRect(QRect(panelW, 0, 120, height()), fadeL);
    }

    int panelW = (int)(width() * 0.42);
    QLinearGradient grad(0, 0, panelW, height());
    grad.setColorAt(0.0, QColor(0xD4, 0xEF, 0xE3));
    grad.setColorAt(1.0, QColor(0xBF, 0xE5, 0xD5));
    painter.fillRect(QRect(0, 0, panelW, height()), grad);

    painter.setRenderHint(QPainter::Antialiasing);
    QPen circlePen(QColor(0x1A, 0x5C, 0x3A, 40), 2);
    painter.setPen(circlePen);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(-80, -80, 280, 280);
    painter.drawEllipse(-40, -40, 200, 200);
}

void MainWindow::setupUI() {
    setWindowTitle("NASE UFPE – Sistema de Atendimento");
    setMinimumSize(820, 560);
    resize(900, 600);

    auto* central = new QWidget(this);
    setCentralWidget(central);
    auto* root = new QHBoxLayout(central);
    root->setContentsMargins(0,0,0,0);
    root->setSpacing(0);

    auto* leftPanel = new QWidget(central);
    leftPanel->setFixedWidth(380);
    leftPanel->setAttribute(Qt::WA_TranslucentBackground);
    leftPanel->setStyleSheet("background: transparent;");

    auto* panelLayout = new QVBoxLayout(leftPanel);
    panelLayout->setContentsMargins(48, 56, 48, 48);
    panelLayout->setSpacing(0);

    auto* lblIcon = new QLabel("🏥", leftPanel);
    lblIcon->setAlignment(Qt::AlignLeft);
    lblIcon->setStyleSheet("font-size: 38px; background: transparent;");

    auto* lblNase = new QLabel("NASE UFPE", leftPanel);
    lblNase->setStyleSheet("font-size:44px;font-weight:900;color:#1A5C3A;letter-spacing:2px;background:transparent;font-family:'Segoe UI','Arial Black',sans-serif;");

    auto* lblSub = new QLabel("NÚCLEO DE ATENÇÃO\nÀ SAÚDE DO ESTUDANTE", leftPanel);
    lblSub->setStyleSheet("font-size:11px;font-weight:700;color:#2E7D52;letter-spacing:1.5px;background:transparent;");

    edtLogin = new QLineEdit(leftPanel);
    edtLogin->setPlaceholderText("  Login");
    edtLogin->setMinimumHeight(46);
    edtLogin->setStyleSheet(R"(
        QLineEdit { background:rgba(255,255,255,0.92); border:1.5px solid rgba(26,92,58,0.18);
            border-radius:8px; font-size:14px; color:#1A3028; padding:0 14px; }
        QLineEdit:focus { border:1.5px solid #2E7D52; background:white; }
    )");

    // Campo senha com botão de mostrar/ocultar senha
    auto* senhaRow    = new QWidget(leftPanel);
    senhaRow->setAttribute(Qt::WA_TranslucentBackground);
    auto* senhaLayout = new QHBoxLayout(senhaRow);
    senhaLayout->setContentsMargins(0,0,0,0);
    senhaLayout->setSpacing(6);

    edtSenha = new QLineEdit(senhaRow);
    edtSenha->setPlaceholderText("  Senha");
    edtSenha->setEchoMode(QLineEdit::Password);
    edtSenha->setMinimumHeight(46);
    edtSenha->setStyleSheet(edtLogin->styleSheet());

    btnVerSenha = new QPushButton("👁", senhaRow);
    btnVerSenha->setFixedSize(46, 46);
    btnVerSenha->setCheckable(true);
    btnVerSenha->setCursor(Qt::PointingHandCursor);
    btnVerSenha->setStyleSheet(R"(
        QPushButton { background-color:rgba(255,255,255,0.85); border:none; border-radius:8px; font-size:18px; }
        QPushButton:hover { background-color:rgba(255,255,255,1.0); }
        QPushButton:checked { background-color:rgba(200,240,220,1.0); }
    )");

    senhaLayout->addWidget(edtSenha);
    senhaLayout->addWidget(btnVerSenha);

    btnLogin = new QPushButton("🔒   LOGIN", leftPanel);
    btnLogin->setMinimumHeight(46);
    btnLogin->setCursor(Qt::PointingHandCursor);
    btnLogin->setStyleSheet(R"(
        QPushButton { background-color:#1A5C3A; color:white; border:none; border-radius:8px;
            font-size:14px; font-weight:bold; letter-spacing:2px; }
        QPushButton:hover { background-color:#226B47; }
        QPushButton:pressed { background-color:#134529; }
    )");

    btnSair = new QPushButton("  SAIR", leftPanel);
    btnSair->setMinimumHeight(44);
    btnSair->setCursor(Qt::PointingHandCursor);
    btnSair->setStyleSheet(R"(
        QPushButton { background-color:transparent; color:#1A5C3A; border:1.5px solid #1A5C3A;
            border-radius:8px; font-size:14px; font-weight:600; letter-spacing:1.5px; }
        QPushButton:hover { background-color:rgba(26,92,58,0.08); }
    )");

    lblErro = new QLabel("", leftPanel);
    lblErro->setStyleSheet("color:#c0392b; font-size:12px; background:transparent;");
    lblErro->setAlignment(Qt::AlignCenter);
    lblErro->hide();

    auto* lblDica = new QLabel("Teste: admin / Admin@123", leftPanel);
    lblDica->setStyleSheet("color:rgba(26,92,58,0.45); font-size:10px; background:transparent;");
    lblDica->setAlignment(Qt::AlignCenter);

    panelLayout->addWidget(lblIcon);
    panelLayout->addSpacing(4);
    panelLayout->addWidget(lblNase);
    panelLayout->addSpacing(2);
    panelLayout->addWidget(lblSub);
    panelLayout->addSpacing(36);
    panelLayout->addWidget(edtLogin);
    panelLayout->addSpacing(10);
    panelLayout->addWidget(senhaRow);
    panelLayout->addSpacing(16);
    panelLayout->addWidget(btnLogin);
    panelLayout->addSpacing(8);
    panelLayout->addWidget(btnSair);
    panelLayout->addSpacing(8);
    panelLayout->addWidget(lblErro);
    panelLayout->addSpacing(4);
    panelLayout->addWidget(lblDica);
    panelLayout->addStretch();

    root->addWidget(leftPanel);
    root->addStretch();

    connect(btnLogin,   &QPushButton::clicked,     this, &MainWindow::onLoginClicked);
    connect(btnSair,    &QPushButton::clicked,     this, &MainWindow::onSairClicked);
    connect(edtSenha,   &QLineEdit::returnPressed, this, &MainWindow::onLoginClicked);
    connect(btnVerSenha,&QPushButton::toggled, this, [this](bool on){
        edtSenha->setEchoMode(on ? QLineEdit::Normal : QLineEdit::Password);
        btnVerSenha->setText(on ? "🙈" : "👁");
    });
}

void MainWindow::onLoginClicked() { // Esconder mensagem de erro anterior 
    lblErro->hide();
    QString login = edtLogin->text().trimmed();
    QString senha = edtSenha->text();

    for (auto u : usuarios) {
        if (u->getLogin() == login.toStdString() && u->autenticar(senha.toStdString())) {
            edtSenha->clear();
            Paciente* p = dynamic_cast<Paciente*>(u);
            if (p) {
                auto* w = new MenuPacienteWindow(p, usuarios, fila, db);
                w->show();
            } else {
                auto* w = new MenuFuncionarioWindow(u, usuarios, fila, db);
                w->show();
            }
            return;
        }
    }
    lblErro->setText("Login ou senha inválidos.");
    lblErro->show();
    edtSenha->clear();
    edtSenha->setFocus();
}

void MainWindow::onSairClicked() { QApplication::quit(); }
