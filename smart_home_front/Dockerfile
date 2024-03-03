FROM node:17-alpine as builder
WORKDIR /app
COPY package.json .
RUN npm install --silent
COPY . .
RUN npm run build

FROM nginx:latest
COPY --from=builder /app/build /usr/share/nginx/html
ENTRYPOINT ["nginx", "-g", "daemon off;"]