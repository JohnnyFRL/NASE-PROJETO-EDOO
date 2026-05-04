#ifndef NASESTYLE_H
#define NASESTYLE_H

static const char* NASE_GLOBAL_STYLE = R"(
QWidget {
    background-color: #F0F7F3;
    color: #1a3a28;
    font-family: 'Segoe UI', sans-serif;
    font-size: 13px;
}
QLineEdit, QTextEdit, QComboBox, QSpinBox {
    background-color: #FFFFFF;
    border: 1.5px solid #C8DDD2;
    border-radius: 6px;
    color: #1a3a28;
    padding: 5px 12px;
}
QLineEdit:focus, QTextEdit:focus, QComboBox:focus { border: 1.5px solid #1B5E35; }
QLineEdit[readOnly="true"] { background-color: #EEF6F1; color: #6B7280; }
QComboBox::drop-down { border: none; width: 24px; }
QLabel { color: #1a3a28; background: transparent; }
QScrollArea { border: none; background: transparent; }
QScrollBar:vertical { background: #E8F5EE; width: 8px; border-radius: 4px; }
QScrollBar::handle:vertical { background: #A8C8B8; border-radius: 4px; min-height: 24px; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
QListWidget {
    background: #FFFFFF; border: 1.5px solid #C8DDD2;
    border-radius: 6px; color: #1a3a28;
}
QListWidget::item { padding: 8px 12px; border-bottom: 1px solid #E8F5EE; }
QListWidget::item:selected { background: #1B5E35; color: white; border-radius: 4px; }
QListWidget::item:hover    { background: #D4EDDF; }
QGroupBox {
    border: 1.5px solid #C8DDD2; border-radius: 8px;
    margin-top: 10px; color: #2E7D52; font-weight: 600; background: #FFFFFF;
}
QGroupBox::title { subcontrol-origin: margin; left: 12px; padding: 0 6px; color: #1B5E35; }
QCheckBox { color: #1a3a28; }
QCheckBox::indicator { width: 16px; height: 16px; border: 1.5px solid #2E7D52; border-radius: 4px; background: white; }
QCheckBox::indicator:checked { background: #1B5E35; border-color: #1B5E35; }
QDialog { background-color: #F0F7F3; }
QMessageBox { background: #FFFFFF; }
QMessageBox QLabel { color: #1a3a28; }
QLabel#lblTitDlg { font-size: 17px; font-weight: bold; color: #1B5E35; }
QLabel#lblErro { color: #DC2626; font-size: 12px; }
QLabel#lblStatusDlg { font-size: 12px; color: #6B7280; }
QPushButton#btnPrim {
    background-color: #1B5E35; color: white; border: none;
    border-radius: 6px; font-weight: bold; padding: 8px 20px; min-height: 36px;
}
QPushButton#btnPrim:hover   { background-color: #2E7D52; }
QPushButton#btnPrim:pressed { background-color: #144d2a; }
QPushButton#btnSec {
    background-color: transparent; color: #1B5E35;
    border: 1.5px solid #1B5E35; border-radius: 6px; padding: 8px 20px; min-height: 36px;
}
QPushButton#btnSec:hover { background-color: #D4EDDF; }
QPushButton#btnNegar {
    background-color: #DC2626; color: white; border: none;
    border-radius: 6px; font-weight: bold; padding: 8px 20px; min-height: 36px;
}
QPushButton#btnNegar:hover { background-color: #b91c1c; }
QPushButton#btnPerigo {
    background-color: transparent; color: #DC2626;
    border: 1.5px solid #DC2626; border-radius: 6px; padding: 8px; min-height: 34px;
}
QPushButton#btnPerigo:hover { background-color: #fef2f2; }
QPushButton#btnAlerta {
    background-color: #F59E0B; color: white; border: none;
    border-radius: 6px; font-weight: bold; padding: 8px 20px; min-height: 36px;
}
QPushButton#btnAlerta:hover { background-color: #d97706; }
QPushButton#btnVoltar {
    background-color: transparent; color: #6B7280;
    border: 1.5px solid #C8DDD2; border-radius: 6px; padding: 8px; min-height: 34px;
}
QPushButton#btnVoltar:hover { color: #DC2626; border-color: #DC2626; }
QPushButton#btnOlho {
    background: #E8F5EE; border: 1.5px solid #C8DDD2; border-radius: 6px; font-size: 16px;
}
QPushButton#btnOlho:checked { background: #1B5E35; border-color: #1B5E35; }
QPushButton#btnSenha {
    background-color: #E8F5EE; color: #1B5E35;
    border: 1.5px solid #C8DDD2; border-radius: 6px; padding: 8px; min-height: 36px;
}
QPushButton#btnSenha:hover { background-color: #D4EDDF; }
)";

#endif
