#ifndef CONNECTOPTIONSDIALOG_H
#define CONNECTOPTIONSDIALOG_H

#include <QDialog>
#include <QMap>
#include <QVector>
#include <QWidget>
#include <QLabel>

struct OptionDef {
    QString name;
    QString label;
    QString tooltip;
    enum Type {
        TypeString,
        TypeNumber,
        TypeBoolean,
        TypeSelect,
        TypePassword
    } type;
    QStringList selectOptions;
    int minValue = 0;
    int maxValue = INT_MAX;

    OptionDef() = default;

    OptionDef(const QString &name,
                  const QString &label,
                  const QString &tooltip,
                  Type type,
                  const QStringList &selectOptions = {},
                  int minValue = 0,
                  int maxValue = INT_MAX)
            : name(name),
              label(label),
              tooltip(tooltip),
              type(type),
              selectOptions(selectOptions),
              minValue(minValue),
              maxValue(maxValue)
        {}
};

class ConnectOptionsDialog : public QDialog {
    Q_OBJECT
public:
    explicit ConnectOptionsDialog(
        const QString &driverName,
        const QString &options = QString(),
        QWidget *parent = nullptr
    );

    ~ConnectOptionsDialog();

    QString getOptionsString() const;

private:
    void buildUi();
    void loadOptionDefinitions();
    QList<OptionDef> getOptionsForDriver(const QString &driver);
    void parseCurrentOptions(const QString &optionsStr);
    void updateResultString();

    private slots:
    void onFieldChanged();
    void onOkClicked();
    void onCancelClicked();

private:
    QString driverName;
    QString currentOptions;
    QMap<QString, QWidget*> optionWidgets;
    QMap<QString, OptionDef> optionDefinitions;
    QLabel *resultLabel;

    static QList<OptionDef> sqlite_options;
    static QList<OptionDef> mysql_options;
    static QList<OptionDef> psql_options;
    static QList<OptionDef> odbc_options;
    static QList<OptionDef> oci_options;
    static QList<OptionDef> db2_options;
};

#endif // CONNECTOPTIONSDIALOG_H
