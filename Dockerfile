FROM hhvm/hhvm-proxygen:3.21.3

RUN mkdir -p /var/www &&\
    chown www-data:www-data /var/www &&\
    chmod 0755 /var/www

COPY ./src/burn.hh /var/www/burn.hh

USER www-data
WORKDIR /var/www

EXPOSE 8080

ENTRYPOINT /usr/bin/hhvm -d hhvm.hack.lang.look_for_typechecker=0 -m server -p 8080 -d hhvm.jit=false
