FROM node

WORKDIR /app
RUN git clone https://github.com/apollographql/fullstack-tutorial.git

WORKDIR /app/fullstack-tutorial/final/server

RUN npm install

ENV PORT=8080
EXPOSE 8080

CMD [ "npm", "start" ]

