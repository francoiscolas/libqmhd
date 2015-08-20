#include "qmhdglobal.h"

#include <QDateTime>
#include <QLocale>

static const char* s_method_strings[] = {
#define XX(name, string) #string,
    QMHD_METHOD_MAP(XX)
#undef XX
    0
};

static int s_method_count = sizeof(s_method_strings) / sizeof(*s_method_strings);

QMHD::Method qmhd_method_from_string(const char* method)
{
    for (int i = 0; s_method_strings[i] != 0; ++i) {
        if (qstricmp(s_method_strings[i], method) == 0)
            return (QMHD::Method) i;
    }
    return QMHD::UnknownMethod;
}

const char* qmhd_method_to_string(QMHD::Method method)
{
    int m = (int) method;

    if (m >= 0 && m < s_method_count)
        return s_method_strings[m];
    return NULL;
}

QMHD::HttpVersion qmhd_http_version_from_string(const char* http_version)
{
    if (qstrcmp("HTTP/1.0", http_version) == 0)
        return QMHD::Http_1_0;
    if (qstrcmp("HTTP/1.1", http_version) == 0)
        return QMHD::Http_1_1;
    return QMHD::UnknownVersion;
}

QString qmhd_header_date(const QDateTime& dt)
{
    return QLocale(QLocale::English).toString(dt.toUTC(), "ddd, dd MMM yyyy hh:mm:ss 'GMT'");
}

QString qmhd_json_date(const QDateTime& dt)
{
    return dt.toUTC().toString(Qt::ISODate);
}
