import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import GraphQlClient 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("GraphQl Example Client")
    property var subscriptions: [];

    /*
        Example queries for the apollo star wars server

        Query:
            query {hero {name}}

        Mutation:
            mutation {createReview(episode: EMPIRE, review: {stars: 3}){episode,stars}}

        Subscription:
            subscription {reviewAdded(episode: EMPIRE){stars}}

    */


    function isNullOrWhitespace( input ) {
      return !input || !input.trim();
    }

    function executeQuery() {
        var query = txtSearchQuery.text;
        if (!isNullOrWhitespace(query)) {
            console.log("execute query: " + query);
            txtResult.text = "";

            if (query.indexOf("mutation") !== -1) {
                var mutationId = gql.mutate(query);
                console.log("mutation id is: " + mutationId);
            } else if (query.indexOf("subscription" !== -1)) {
                var subscriptionId = gql.query(query);
                console.log("subscription id is: " + subscriptionId);
                subscriptions.push(subscriptionId);
                console.log(subscriptions.length)
                txtResult.text = "subscribed " + query;
            } else {
                var queryId = gql.query(query);
                console.log("query id is: " + queryId);
            }
        } else {
            txtResult.text = "";
        }
    }

    function unsubscribeAllSubscriptions() {
        console.log(subscriptions.length);
        for(var i = 0; i < subscriptions.length; ++i) {
            var id = subscriptions[i];
            console.log("unsubscribe " + id);
            gql.unsubscribe(id);
        }
        subscriptions = [];
        txtResult.text = "unsubscribed from all subscriptions";
    }

    GraphQlConnection {
        id: gql
        url: "ws://localhost:9000/websocket"  //using a websocket connection
        //url: "http://localhost:9000/graphql"    //using a http connection

        //if you are using a websocket connection use this to open the connection
        Component.onCompleted: {
            console.log("completed");
            open();
        }

        onDataReceived: {
            //result data is available as json object
            var resultAsJson = JSON.stringify(data, /*replacer*/ null, /*spacing*/ 2);
            console.log(resultAsJson)
            console.log("response from query with id:" + data.id);

            if (data.payload.errors) {
                console.log("having errors");
                txtResult.text = resultAsJson
            } else {

                //name from query: query {hero {name}}
                //console.log(data.payload.data.hero.name);

                //e.g. you can access data from the example query like this
                //console.log(data.payload.data.allFilms.films[0].title);

                //or like this
                //console.log(data["payload"]["data"]["allFilms"]["films"][0]["title"])
                console.log(subscriptions.indexOf(data.id));
                if (subscriptions.indexOf(data.id) > 0) {
                    console.log("received a subscription");
                    txtResult.text = "=== Data from subscription: ===\n\n" +  resultAsJson + "\n\n=== End data from subscription";
                } else {
                    txtResult.text = resultAsJson
                }
            }
        }

        onError: {
            var resultAsJson = JSON.stringify(error, /*replacer*/ null, /*spacing*/ 2);
            console.log(resultAsJson)

            console.log("Error: " + error.message);
            var msg = "Error:\n\n" + error.message;
            txtResult.text = msg
        }
    }

    ColumnLayout {
        spacing: 4
        anchors.fill: parent
        Layout.leftMargin: 5
        Layout.fillWidth:  true
        RowLayout {
            Layout.rightMargin: 5
            Layout.leftMargin: 5
            Rectangle {
                anchors.topMargin: 5
                anchors.leftMargin: 5
                height: txtSearchQuery.height
                Layout.fillWidth: true
                color: "#3b6e94"
                radius: 5

                TextArea {
                    id: txtSearchQuery
                    anchors.fill: parent
                    font.pixelSize: 14
                    color: "#2e2e34"
                    placeholderText: "Enter your query here"
                    text: "query {hero {name}}"
                    Keys.onReturnPressed: executeSearch();
                }
            }

            Button {
                text: "Execute query"
                Layout.preferredHeight: txtSearchQuery.height
                font.pixelSize: 14
                onClicked: executeQuery();
            }

            Button {
                text: "Unsubscribe All"
                Layout.preferredHeight: txtSearchQuery.height
                font.pixelSize: 14
                onClicked: unsubscribeAllSubscriptions();
            }

        } //RowLayout

        Rectangle {
            color: "#3b6e94"
            Layout.fillHeight: true
            Layout.fillWidth:  true

            ColumnLayout {
                anchors.fill: parent
                Layout.fillHeight: true
                Layout.fillWidth:  true

                Text {
                    Layout.topMargin: 10
                    Layout.leftMargin: 5
                    text: qsTr("Result as json:")
                    font.pixelSize: 14
                    color: "#2e2e34"
                }

                Rectangle {
                    height: 1
                    Layout.fillWidth: true
                    color: "#000000"
                }

                ScrollView {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    TextArea {
                        id:txtResult
                        font.pointSize: 12
                        color: "#2e2e34"
                    }
                }
            }//ColumnLayout
        }

    }//ColumnLayout
}
