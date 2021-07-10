#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
  int sock = 0;
  struct sockaddr_in serv_addr;

  fd_set rfds;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Erro criando socket! \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
  {
    printf("\nEndereço Inválido!/ Não suportado...\n");
    return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nConexão falhou!\n");
    return -1;
  }

  int flags;
  flags = fcntl(sock, F_GETFL, 0);
  assert(flags != -1);
  fcntl(sock, F_SETFL, flags | O_NONBLOCK);

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
    FD_SET(sock, &read_flags);
    FD_SET(sock, &write_flags);
    FD_SET(STDIN_FILENO, &read_flags);
    FD_SET(STDIN_FILENO, &write_flags);

    sel = select(sock + 1, &read_flags, &write_flags, NULL, NULL);

    if (FD_ISSET(sock, &read_flags))
    {
      FD_CLR(sock, &read_flags);

      memset(&in, 0, 255);
      read(sock, in, 255);

      printf("\nServer: %s", in);
    }

    if (FD_ISSET(STDIN_FILENO, &read_flags))
    {
      fgets(out, 200, stdin);
    }

    if (FD_ISSET(sock, &write_flags))
    {
      FD_CLR(sock, &write_flags);

      send(sock, out, strlen(out), 0);
      memset(&out, 0, 255);
    }
  }

  return 0;
}