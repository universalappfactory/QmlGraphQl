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

    function isNullOrWhitespace( input ) {
      return !input || !input.trim();
    }

    function executeSearch() {
        var query = txtSearchQuery.text;
        if (!isNullOrWhitespace(query)) {
            console.log("execute query: " + query);
            gql.query(query);
        } else {
            txtResult.text = "";
        }
    }

    GraphQlConnection {
        id: gql
        url: "http://localhost:9000/graphql"
        //url: "http://localhost:9000/"

        onDataReceived: {
            //result data is available as json object
            var resultAsJson = JSON.stringify(data, /*replacer*/ null, /*spacing*/ 2);
            console.log(resultAsJson)

            //e.g. you can access data from the example query like this
            //console.log(data.data.allFilms.films[0].title);

            //or like this
            //console.log(data["data"]["allFilms"]["films"][0]["title"])

            txtResult.text = resultAsJson
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
                    text: "query { allFilms {  films {title},  pageInfo {hasNextPage}}}"
                    Keys.onReturnPressed: executeSearch();
                }
            }

            Button {
                text: "Execute query"
                Layout.preferredHeight: txtSearchQuery.height
                font.pixelSize: 14
                onClicked: executeSearch();
            }

            //Example for a mutation (this is a query for the apollo graphql starwars server)
            /*
            Button {
                text: "Mutate"
                Layout.preferredHeight: txtSearchQuery.height
                font.pixelSize: 14
                onClicked: {
                    var mutation = "mutation {createReview(episode: EMPIRE, review: {stars: 3}){episode,stars}}";
                    console.log("mutate: " + mutation);
                    gql.mutate(mutation);
                }
            }
            */

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
