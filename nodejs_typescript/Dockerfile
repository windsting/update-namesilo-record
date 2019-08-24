FROM node:10-alpine as builder                                                                                                                            
WORKDIR /app
COPY package*.json ./
RUN npm ci
COPY . .
RUN npm run build

# only need node_modules and "dist" directory to run
FROM node:10-alpine
WORKDIR /app
COPY package*.json ./
RUN npm ci --only=production
COPY --from=builder /app/dist ./dist
CMD ["node", "dist/index.js"]
