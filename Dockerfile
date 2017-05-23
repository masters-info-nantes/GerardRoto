FROM ubuntu:latest

# Update Base System
RUN apt-get update 

RUN apt-get install -y \
    git \
    make \
    g++ \
    qt5-qmake \
    qt5-default \
    tar \
    ffmpeg


# Install Fake X11 display to connect with VNC
RUN apt-get install -y \
    x11vnc \
    xvfb 

RUN mkdir ~/.vnc
RUN x11vnc -storepasswd 1234 ~/.vnc/passwd

# Install project
WORKDIR /root
# RUN git clone https://jeremy-lr@bitbucket.org/jeremy-lr/etude-de-cas.git
# RUN qmake -makefile GerardRoto.pro
# RUN make
# ./GerardRoto

# docker build  -t project/gerardroto ./  
# docker run --name dev -it --rm -v /home/jbardon/Documents/GerardRoto:/root -p 5900 project/gerardroto x11vnc -forever -usepw -create
# docker inspect dev | grep IPAddress

