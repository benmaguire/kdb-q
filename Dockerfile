FROM ubuntu:latest 
 
RUN dpkg --add-architecture i386
RUN apt-get update ; apt-get install -y unzip libc6:i386 libncurses5:i386 libstdc++6:i386
RUN apt-get install -y libzmq5:i386 libjson-c4:i386

COPY linuxx86.zip /kx/linuxx86.zip
COPY docker-entrypoint.sh /docker-entrypoint.sh
COPY bin/ /kx/q/l32/
COPY bin_deb/ /tmp/bin_deb

RUN dpkg -i /tmp/bin_deb/multiarch-support_2.27-3ubuntu1_i386.deb
RUN dpkg -i /tmp/bin_deb/libzookeeper-mt2_3.4.10-3_i386.deb
RUN dpkg -i /tmp/bin_deb/libjson-c2_0.11-4ubuntu2_i386.deb
RUN unzip /kx/linuxx86.zip -d /kx
RUN chmod 755 /docker-entrypoint.sh /kx/q/l32/q
RUN rm /kx/linuxx86.zip ; rm /tmp/bin_deb/*
 
RUN apt-get remove -y unzip
 
CMD ["/docker-entrypoint.sh"] 
 