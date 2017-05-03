FROM resin/raspberrypi3-debian

RUN apt-get update
RUN apt-get install -y wget
RUN wget -q http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key
RUN apt-key add mosquitto-repo.gpg.key
RUN cd /etc/apt/sources.list.d
RUN wget http://repo.mosquitto.org/debian/mosquitto-jessie.list

RUN apt-get update
RUN apt-get install -y mosquitto

CMD ["/usr/sbin/mosquitto"]