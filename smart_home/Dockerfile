FROM maven:3.9-eclipse-temurin-21-alpine AS MAVEN_BUILD
COPY pom.xml /build/
COPY src /build/src/
WORKDIR /build/
RUN mvn package

FROM eclipse-temurin:21-alpine
WORKDIR /app
COPY --from=MAVEN_BUILD /build/target/*.jar /app/application.jar
ENTRYPOINT ["java", "-jar", "application.jar"]