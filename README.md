# QmlGraphQl
Is a simple GraphQL Client for QT and QML.

You can easily use this client within QML.

It's under development at the moment, so don't consider this as a stable version.

But querying works pretty good at the moment.

The client supports http and websocket connections.

The websocket connection was implemented against a [HotChocolate Server](https://github.com/ChilliCream/hotchocolate)

Next I'd like to implement mutations and subscriptions.

# Usage

The client is implemented as [QQmlExtensionPlugin](http://doc.qt.io/qt-5/qqmlextensionplugin.html)

```QML

//import the plugin
import GraphQlClient 1.0

//...

    GraphQlConnection {
        id: gql
        url: "http://localhost:9000/"

        onDataReceived: {
            //result data is available as json object
            var resultAsJson = JSON.stringify(data, /*replacer*/ null, /*spacing*/ 2);
            console.log(resultAsJson)

            //e.g. you can access data from the example query like this
            //console.log(data.data.allFilms.films[0].title);

            //or like this
            //console.log(data["data"]["allFilms"]["films"][0]["title"])
        }
    }

    //Execute a query
    Button 
    {
        text: "Execute query"
        Layout.preferredHeight: txtSearchQuery.height
        font.pixelSize: 14

        onClicked: {
            gql.query("query { allFilms {  films {title},  pageInfo {hasNextPage}}");                    
        }
    }

//...

```

# Starting a GraphQL Server using Docker

If you want to start a graphql example server locally using docker you may use one of the two docker files to build a docker container.

The first one uses the [SWAPI GraphQL Wrapper](https://github.com/graphql/swapi-graphql)

The second one the [Apollo GraphQL server example](https://github.com/apollographql/graphql-server-example)


```Bash
#go to the root directory of the repository
docker build SwapiDocker -t graphql/swapi

#or
docker build ApolloGraphQLDocker -t graphql/apollo

#run (available under localhost:9000/playground)
docker run -p 9000:8080 graphql/swapi

#or (available under localhost:9000/graphql)
docker run -p 9000:4000 graphql/apollo

```

# The Example App

To create the example app, you need to open both projects (ExampleApp.pro and graphqlclient.pro) in QtCreator.

Then build the graphqlclient.pro, this will create a directory "GraphQlClient" under the ExampleApp folder. This is necessary so that the ExampleApp is able to find the plugin.

(See <http://doc.qt.io/qt-5/qtqml-modules-cppplugins.html>)


# Issues

Code completition for client properties does not work properly, even if I created a qmltypes file for the plugin.

Error handling for the httpclient isn't implemented yet.



