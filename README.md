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

        onError: {
            var resultAsJson = JSON.stringify(error, /*replacer*/ null, /*spacing*/ 2);
            console.log(resultAsJson)

            console.log("Error: " + error.message);
            var msg = "Error:\n\n" + error.message;
            txtResult.text = msg
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

If you want to start a graphql example server locally using docker, you may use one of the docker files to build a docker container.

The first one uses the [SWAPI GraphQL Wrapper](https://github.com/graphql/swapi-graphql)

The second one the [Apollo GraphQL server example](https://github.com/apollographql/graphql-server-example)

And the third one the [Apollo GraphQL star-wars server example](https://github.com/apollographql/starwars-server)


```Bash
#go to the root directory of the repository
docker build SwapiDocker -t graphql/swapi

#or
docker build ApolloGraphQLDocker -t graphql/apollo

#or
docker build ApolloStarWarsGraphQLDocker -t apollo/starwars


#run (graphiql is available under localhost:9000/playground, use localhost:9000 as url for the client)
docker run -p 9000:8080 graphql/swapi

#or (graphiql is available under localhost:9000/playground, use localhost:9000 as url for the client)
docker run -p 9000:8080 graphql/apollo

#or (graphiql is available under http://localhost:9000/graphql, use http://localhost:9000/graphql as url for the client)
docker run -p 9000:8080 apollo/starwars

```

# The Example App

To create the example app, you need to open both projects (ExampleApp.pro and graphqlclient.pro) in QtCreator.

Then build the graphqlclient.pro, this will create a directory "GraphQlClient" under the ExampleApp folder. This is necessary so that the ExampleApp is able to find the plugin.
I disabled the "Shadow Build" for both, example app and graphqlclient.

(See <http://doc.qt.io/qt-5/qtqml-modules-cppplugins.html>)

# Mutations

A mutation query to the server is pretty much the same as a "normal" query.
E.g with the apollo star wars server you can already do a mutation like this:

```QML
    ...
    
    gql.query("mutation {createReview(episode: EMPIRE, review: {stars: 3}){episode,stars}}")

    ...
```

But in order to have a distinction in the ui there is also a "mutate" function.

```QML
    ...
    
    gql.mutate("mutation {createReview(episode: EMPIRE, review: {stars: 3}){episode,stars}}")

    ...
```


# Issues

Sometimes I have some problems with code completition for the plugin in QtCreator

Query variables are not working at the moment

~~Error handling for the httpclient isn't implemented yet.~~



