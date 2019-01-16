#include "graphqlclient_plugin.h"
#include "myitem.h"
#include "graphqlconnection.h"

#include <qqml.h>

void GraphQlClientPlugin::registerTypes(const char *uri)
{
    // @uri com.mycompany.qmlcomponents
    qmlRegisterType<MyItem>(uri, 1, 0, "MyItem");
    qmlRegisterType<GraphQlConnection>(uri, 1, 0, "GraphQlConnection");
}

