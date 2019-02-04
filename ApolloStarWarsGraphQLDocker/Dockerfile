FROM node

WORKDIR /app
RUN git clone https://github.com/apollographql/starwars-server

WORKDIR /app/starwars-server

RUN npm install

ENV PORT=8080
EXPOSE 8080

CMD [ "npm", "start" ]

