FROM node

WORKDIR /app
RUN git clone https://github.com/apollographql/graphql-server-example.git

WORKDIR /app/graphql-server-example

RUN npm install
#RUN npm run build
#RUN npm run download


ENV PORT=8080
EXPOSE 8080

CMD [ "node", "index.js" ]

