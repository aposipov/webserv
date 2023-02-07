
FROM        debian:buster

RUN         apt update && apt install -y \
            --no-install-recommends \
            nginx procps\
            && rm -rf /var/lib/apt/lists/*

COPY        default /etc/nginx/sites-available/default

#ENTRYPOINT  ["nginx", "-g", "daemon off;"]

CMD  ["nginx", "-g", "daemon off;"]