FROM node

WORKDIR /app
RUN git clone https://github.com/graphql/swapi-graphql.git

WORKDIR /app/swapi-graphql

RUN npm install
RUN npm run build
RUN npm run download


ENV PORT=8080
EXPOSE 8080

CMD [ "npm", "start" ]

