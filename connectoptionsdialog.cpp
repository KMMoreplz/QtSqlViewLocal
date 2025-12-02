#include "connectoptionsdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QFormLayout>


// SQLite
QList<OptionDef> ConnectOptionsDialog::sqlite_options = {
    { "QSQLITE_BUSY_TIMEOUT", "Busy Timeout (ms)",
      "Таймаут ожидания блокировки БД в миллисекундах",
      OptionDef::TypeNumber, {}, 0, 300000 },

    { "QSQLITE_OPEN_READONLY", "Открыть только для чтения",
      "Открыть БД только для чтения (не создаст файл, если его нет)",
      OptionDef::TypeBoolean },

    { "QSQLITE_OPEN_URI", "Открытие как URI",
      "Интерпретировать путь как URI вместо обычного пути к файлу",
      OptionDef::TypeBoolean },

    { "QSQLITE_ENABLE_SHARED_CACHE", "Shared Cache",
      "Использовать режим shared cache (по умолчанию: private)",
      OptionDef::TypeBoolean },

    { "QSQLITE_ENABLE_REGEXP", "Поддержка REGEXP",
      "Определить функцию regex() для запросов (значение – размер кэша, умолч. 25)",
      OptionDef::TypeNumber, {}, 0, 1000 }
};

// MySQL
QList<OptionDef> ConnectOptionsDialog::mysql_options = {
    { "CLIENT_COMPRESS", "Compressed Protocol",
      "Переключиться на сжатый протокол после аутентификации",
      OptionDef::TypeBoolean },

    { "CLIENT_FOUND_ROWS", "Found Rows Instead",
      "Отправлять количество найденных строк вместо изменённых",
      OptionDef::TypeBoolean },

    { "CLIENT_IGNORE_SPACE", "Ignore Spaces",
      "Разрешить пробелы перед '(' в имени функции",
      OptionDef::TypeBoolean },

    { "CLIENT_ODBC", "ODBC Compatibility",
      "Включить режим совместимости ODBC",
      OptionDef::TypeBoolean },

    { "CLIENT_NO_SCHEMA", "No Schema",
      "Запретить синтаксис database.table.column",
      OptionDef::TypeBoolean },

    { "CLIENT_INTERACTIVE", "Interactive Client",
      "Клиент рассматривается как интерактивный",
      OptionDef::TypeBoolean },

    { "UNIX_SOCKET", "Socket/Named Pipe",
      "Путь к сокету (Unix) или named pipe (Windows)",
      OptionDef::TypeString },

    { "MYSQL_OPT_RECONNECT", "Auto-Reconnect",
      "Автоматически переподключаться при потере соединения",
      OptionDef::TypeBoolean },

    { "MYSQL_OPT_CONNECT_TIMEOUT", "Connect Timeout (s)",
      "Таймаут подключения в секундах",
      OptionDef::TypeNumber, {}, 1, 300 },

    { "MYSQL_OPT_READ_TIMEOUT", "Read Timeout (s)",
      "Таймаут чтения в секундах",
      OptionDef::TypeNumber, {}, 1, 300 },

    { "MYSQL_OPT_WRITE_TIMEOUT", "Write Timeout (s)",
      "Таймаут записи в секундах",
      OptionDef::TypeNumber, {}, 1, 300 },

    { "SSL_KEY", "SSL Key File",
      "Путь к файлу приватного ключа клиента",
      OptionDef::TypeString },

    { "SSL_CERT", "SSL Certificate",
      "Путь к файлу сертификата клиента",
      OptionDef::TypeString },

    { "SSL_CA", "SSL CA Certificate",
      "Путь к файлу CA сертификата",
      OptionDef::TypeString },

    { "SSL_CAPATH", "SSL CA Path",
      "Путь к директории с CA сертификатами",
      OptionDef::TypeString },

    { "SSL_CIPHER", "SSL Cipher List",
      "Список допустимых SSL шифров",
      OptionDef::TypeString }
};

// PostgreSQL
QList<OptionDef> ConnectOptionsDialog::psql_options = {
    { "connect_timeout", "Connect Timeout (s)",
      "Таймаут подключения в секундах",
      OptionDef::TypeNumber, {}, 0, 300 },

    { "options", "Options",
      "Дополнительные опции",
      OptionDef::TypeString },

    { "tty", "TTY",
      "TTY для вывода debug сообщений",
      OptionDef::TypeString },

    { "requiressl", "Require SSL",
      "Требовать SSL соединение",
      OptionDef::TypeBoolean },

    { "service", "Service Name",
      "Имя сервиса из файла .pg_service.conf",
      OptionDef::TypeString }
};

// ODBC
QList<OptionDef> ConnectOptionsDialog::odbc_options = {
    { "SQL_ATTR_ACCESS_MODE", "Access Mode",
      "Режим доступа (Read-only или Read-Write)",
      OptionDef::TypeSelect,
      QStringList() << "SQL_MODE_READ_ONLY" << "SQL_MODE_READ_WRITE" },

    { "SQL_ATTR_LOGIN_TIMEOUT", "Login Timeout (s)",
      "Таймаут входа в секундах (0: бесконечно)",
      OptionDef::TypeNumber, {}, 0, 32767 },

    { "SQL_ATTR_CONNECTION_TIMEOUT", "Connection Timeout (s)",
      "Таймаут для любого запроса в секундах (0: бесконечно)",
      OptionDef::TypeNumber, {}, 0, 32767 },

    { "SQL_ATTR_CURRENT_CATALOG", "Database/Catalog",
      "Каталог (БД) для использования",
      OptionDef::TypeString },

    { "SQL_ATTR_METADATA_ID", "Metadata ID",
      "Обработка строк как идентификаторов",
      OptionDef::TypeSelect,
      QStringList() << "SQL_TRUE" << "SQL_FALSE" },

    { "SQL_ATTR_PACKET_SIZE", "Packet Size (bytes)",
      "Размер сетевого пакета",
      OptionDef::TypeNumber, {}, 512, 1000000 },

    { "SQL_ATTR_TRACEFILE", "Trace File",
      "Путь к файлу трассировки для отладки",
      OptionDef::TypeString },

    { "SQL_ATTR_TRACE", "Enable Tracing",
      "Включить или отключить трассировку запросов",
      OptionDef::TypeSelect,
      QStringList() << "SQL_OPT_TRACE_OFF" << "SQL_OPT_TRACE_ON" },

    { "SQL_ATTR_CONNECTION_POOLING", "Connection Pooling",
      "Режим пула соединений",
      OptionDef::TypeSelect,
      QStringList() << "SQL_CP_OFF" << "SQL_CP_ONE_PER_DRIVER" << "SQL_CP_ONE_PER_HENV" },

    { "SQL_ATTR_ODBC_VERSION", "ODBC Version",
      "Версия ODBC для совместимости",
      OptionDef::TypeSelect,
      QStringList() << "SQL_OV_ODBC2" << "SQL_OV_ODBC3" }
};

// Oracle OCI
QList<OptionDef> ConnectOptionsDialog::oci_options = {
    { "OCI_ATTR_PREFETCH_ROWS", "Prefetch Rows",
      "Количество предварительно загружаемых строк",
      OptionDef::TypeNumber, {}, 1, 10000 },

    { "OCI_ATTR_PREFETCH_MEMORY", "Prefetch Memory (KB)",
      "Размер памяти для prefetch в KB",
      OptionDef::TypeNumber, {}, 1, 100000 }
};

// DB2
QList<OptionDef> ConnectOptionsDialog::db2_options = {
    { "SQL_ATTR_ACCESS_MODE", "Access Mode",
      "Режим доступа (Read-only или Read-Write)",
      OptionDef::TypeSelect,
      QStringList() << "SQL_MODE_READ_ONLY" << "SQL_MODE_READ_WRITE" },

    { "SQL_ATTR_LOGIN_TIMEOUT", "Login Timeout (s)",
      "Таймаут входа в секундах (0: бесконечно)",
      OptionDef::TypeNumber, {}, 0, 32767 }
};


ConnectOptionsDialog::ConnectOptionsDialog(
    const QString &driverName,
    const QString &options,
    QWidget *parent
) : QDialog(parent), driverName(driverName), currentOptions(options)
{
    setWindowTitle(QString("Connect Options - %1").arg(driverName));
    setMinimumWidth(600);
    setMinimumHeight(400);

    buildUi();
    parseCurrentOptions(currentOptions);
}

ConnectOptionsDialog::~ConnectOptionsDialog() {}

void ConnectOptionsDialog::buildUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *driverLayout = new QHBoxLayout;
    QLabel *driverLabel = new QLabel("Database Driver:", this);
    QLineEdit *driverDisplay = new QLineEdit(this);
    driverDisplay->setText(driverName);
    driverDisplay->setReadOnly(true);
    driverDisplay->setStyleSheet("background-color: #f0f0f0; color: #333;");
    driverLayout->addWidget(driverLabel);
    driverLayout->addWidget(driverDisplay);
    mainLayout->addLayout(driverLayout);

    mainLayout->addSpacing(10);

    QList<OptionDef> options = getOptionsForDriver(driverName);

    if (options.isEmpty()) {
        QLabel *noOptions = new QLabel(
            QString("No known options for driver %1").arg(driverName), this
        );
        noOptions->setStyleSheet("color: #888; font-style: italic;");
        mainLayout->addWidget(noOptions);
    } else {

        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setWidgetResizable(true);

        QWidget *scrollWidget = new QWidget;
        QFormLayout *formLayout = new QFormLayout(scrollWidget);

        for (const OptionDef &opt : options) {

            optionDefinitions[opt.name] = opt;

            QLabel *label = new QLabel(opt.label + ":", this);
            label->setToolTip(opt.tooltip);

            QWidget *input = nullptr;

            if (opt.type == OptionDef::TypeBoolean) {
                QCheckBox *cb = new QCheckBox(this);
                cb->setToolTip(opt.tooltip);
                connect(cb, &QCheckBox::stateChanged, this, &ConnectOptionsDialog::onFieldChanged);
                input = cb;
                optionWidgets[opt.name] = cb;

            } else if (opt.type == OptionDef::TypeNumber) {
                QSpinBox *sb = new QSpinBox(this);
                sb->setMinimum(opt.minValue);
                sb->setMaximum(opt.maxValue);
                sb->setToolTip(opt.tooltip);
                connect(sb, QOverload<int>::of(&QSpinBox::valueChanged),
                        this, &ConnectOptionsDialog::onFieldChanged);
                input = sb;
                optionWidgets[opt.name] = sb;

            } else if (opt.type == OptionDef::TypeSelect) {
                QComboBox *cb = new QComboBox(this);
                cb->addItem("-- Not set --", "");
                for (const QString &val : opt.selectOptions) {
                    cb->addItem(val, val);
                }
                cb->setToolTip(opt.tooltip);
                connect(cb, QOverload<int>::of(&QComboBox::currentIndexChanged),
                        this, &ConnectOptionsDialog::onFieldChanged);
                input = cb;
                optionWidgets[opt.name] = cb;

            } else if (opt.type == OptionDef::TypePassword) {
                QLineEdit *le = new QLineEdit(this);
                le->setEchoMode(QLineEdit::Password);
                le->setToolTip(opt.tooltip);
                connect(le, &QLineEdit::textChanged,
                        this, &ConnectOptionsDialog::onFieldChanged);
                input = le;
                optionWidgets[opt.name] = le;

            } else {  // TypeString
                QLineEdit *le = new QLineEdit(this);
                le->setToolTip(opt.tooltip);
                connect(le, &QLineEdit::textChanged,
                        this, &ConnectOptionsDialog::onFieldChanged);
                input = le;
                optionWidgets[opt.name] = le;
            }

            if (input) {
                formLayout->addRow(label, input);
            }
        }

        scrollArea->setWidget(scrollWidget); //TODO: fix
        mainLayout->addWidget(scrollArea);
    }

    // Result section
    mainLayout->addSpacing(10);
    QGroupBox *resultGroup = new QGroupBox("Generated Options String", this);
    QVBoxLayout *resultLayout = new QVBoxLayout(resultGroup);
    resultLabel = new QLabel("(empty)", this);
    resultLabel->setStyleSheet("background-color: #f9f9f9; padding: 8px; border: 1px solid #ddd; border-radius: 3px; font-family: 'Courier New', monospace; font-size: 11px;");
    resultLabel->setWordWrap(true);
    resultLabel->setEnabled(true);
    resultLayout->addWidget(resultLabel);
    mainLayout->addWidget(resultGroup);

    // Buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this
    );
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ConnectOptionsDialog::onOkClicked);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ConnectOptionsDialog::onCancelClicked);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

//Options
QList<OptionDef> ConnectOptionsDialog::getOptionsForDriver(const QString &driver) {
    if (driver == "QSQLITE") return sqlite_options;
    if (driver == "QMYSQL") return mysql_options;
    if (driver == "QPSQL" || "QPSQL7") return psql_options;
    if (driver == "QODBC") return odbc_options;
    if (driver == "QOCI") return oci_options;
    if (driver == "QDB2") return db2_options;
    return QList<OptionDef>();
}

void ConnectOptionsDialog::parseCurrentOptions(const QString &optionsStr) {
    if (optionsStr.isEmpty()) return;

    QStringList pairs = optionsStr.split(';', Qt::SkipEmptyParts);
    for (const QString &pair : pairs) {
        int eqPos = pair.indexOf('=');
        if (eqPos <= 0) continue;

        QString key = pair.left(eqPos).trimmed();
        QString value = pair.mid(eqPos + 1).trimmed();

        if (!optionWidgets.contains(key)) continue;

        QWidget *widget = optionWidgets[key];

        if (auto *cb = qobject_cast<QCheckBox*>(widget)) {
            cb->setChecked(value == "1" || value.toLower() == "true");
        } else if (auto *sb = qobject_cast<QSpinBox*>(widget)) {
            sb->setValue(value.toInt());
        } else if (auto *combo = qobject_cast<QComboBox*>(widget)) {
            combo->setCurrentText(value);
        } else if (auto *le = qobject_cast<QLineEdit*>(widget)) {
            le->setText(value);
        }
    }
}

void ConnectOptionsDialog::updateResultString() {
    QStringList pairs;

    for (auto it = optionWidgets.begin(); it != optionWidgets.end(); ++it) {
        QString key = it.key();
        QWidget *widget = it.value();
        QString value;

        if (auto *cb = qobject_cast<QCheckBox*>(widget)) {
            if (cb->isChecked()) value = "1";
        } else if (auto *sb = qobject_cast<QSpinBox*>(widget)) {
            if (sb->value() > 0 || optionDefinitions[key].minValue == 0) {
                value = QString::number(sb->value());
            }
        } else if (auto *combo = qobject_cast<QComboBox*>(widget)) {
            value = combo->currentData().toString();
        } else if (auto *le = qobject_cast<QLineEdit*>(widget)) {
            value = le->text();
        }

        if (!value.isEmpty()) {
            pairs << QString("%1=%2").arg(key, value);
        }
    }

    QString result = pairs.join(';');
    resultLabel->setText(result.isEmpty() ? "(empty)" : result);
}

QString ConnectOptionsDialog::getOptionsString() const {
    return resultLabel->text() == "(empty)" ? QString() : resultLabel->text();
}

//SLOTS
void ConnectOptionsDialog::onFieldChanged() {
    updateResultString();
}

void ConnectOptionsDialog::onOkClicked() {
    accept();
}

void ConnectOptionsDialog::onCancelClicked() {
    reject();
}
