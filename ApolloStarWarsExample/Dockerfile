FROM node

WORKDIR /app
COPY starwars.tar.gz /app/starwars.tar.gz
RUN tar -xzf starwars.tar.gz

WORKDIR /app/starwars-server

RUN npm install

ENV PORT=8080
EXPOSE 8080

CMD [ "npm", "start" ]