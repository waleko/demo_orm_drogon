FROM postgres
ENV POSTGRES_DB postgres
ENV POSTGRES_USER postgres
ENV POSTGRES_PASSWORD postgres
COPY ./resources/postgres-init.sql /docker-entrypoint-initdb.d/
