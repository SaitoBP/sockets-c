#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
  int server_fd, new_socket;
  struct sockaddr_in address;

  int opt = 1;
  int addrlen = sizeof(address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("Erro abrindo socket");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("Erro ao fazer bind");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
  {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  int flags;
  flags = fcntl(new_socket, F_GETFL, 0);
  assert(flags != -1);
  fcntl(new_socket, F_SETFL, flags | O_NONBLOCK);

  fd_set read_flags, write_flags;
  int sel, size;

  char in[255];
  char out[255];
  memset(&in, 0, 255);
  memset(&out, 0, 255);

  while (1)
  {
    FD_ZERO(&read_flags);
    FD_ZERO(&write_flags);
    FD_SET(new_socket, &read_flags);
    FD_SET(new_socket, &write_flags);
    FD_SET(STDIN_FILENO, &read_flags);
    FD_SET(STDIN_FILENO, &write_flags);

    sel = select(new_socket + 1, &read_flags, &write_flags, NULL, NULL);

    if (FD_ISSET(new_socket, &read_flags))
    {
      FD_CLR(new_socket, &read_flags);

      memset(&in, 0, 255);
      read(new_socket, in, 255);

      printf("\nClient: %s", in);
    }

    if (FD_ISSET(STDIN_FILENO, &read_flags))
    {
      fgets(out, 200, stdin);
    }

    if (FD_ISSET(new_socket, &write_flags))
    {
      FD_CLR(new_socket, &write_flags);

      send(new_socket, out, strlen(out), 0);
      memset(&out, 0, 255);
    }
  }
  return 0;
}