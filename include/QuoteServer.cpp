//
// Created by charles on 11/17/18.
//

#include "QuoteServer.hpp"


size_t file_size(char* filename) {
    size_t fd = open(filename, O_RDONLY);
    struct stat file_stat;
    fstat(fd, &file_stat);
    close(fd);
    return file_stat.st_size;
}


void
QuoteServer::bind_and_listen() {
  size_t ret_in;
  time_t ticks;
  socklen_t len;

  listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr_, '0', sizeof(serv_addr_));

  serv_addr_.sin_family = AF_INET;
  serv_addr_.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  serv_addr_.sin_port = htons(port_);

  bind(listenfd_, (struct sockaddr*)&serv_addr_, sizeof(serv_addr_));

  listen(listenfd_, 5);

  len = sizeof(serv_addr_);
  if (getsockname( listenfd_, (struct sockaddr  *)&serv_addr_, &len) == -1){
      perror("getsockname");
  }

  assigned_port_ = ntohs(serv_addr_.sin_port);
}

int
QuoteServer::send(int num, int fd) {
  int converted_number = htonl(num);

  write(fd, &converted_number, sizeof(converted_number));
  return 0;
}


void
QuoteServer::poll(short max_served) {

  int ret_in;
  // time_t ticks;
  bool tight_wait = true;
  short served = 0;

  while (tight_wait) {
      connfd_ = accept(listenfd_, (struct sockaddr *) NULL, NULL);

      // Do this everytime in a tight loop so that we
      // can detect when underlying file changes
      size_t f_size = file_size(filename_to_serve_);

      // char *sendBuff;
      // sendBuff = (char *)malloc(f_size * (size_t)sizeof(char));

      char sendBuff[f_size];
      memset(sendBuff, '0', f_size);

      size_t input_fd = open(filename_to_serve_, O_RDONLY);
      if (input_fd == -1) {
          perror("open");
          exit(EXIT_FAILURE);
      }

      // ticks = time(NULL);

      struct stat file_stat;
      fstat(input_fd, &file_stat);

      // Send filesize information
      send(f_size, connfd_);

      // Send file
      while ((ret_in = read(input_fd, &sendBuff, file_stat.st_size)) > 0) {
          write(connfd_, sendBuff, file_stat.st_size);
          served++;
          if ((max_served > 0) && (served >= max_served)) {
              tight_wait = false;
          }
      }

      // snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));

      close(connfd_);
  }
}

