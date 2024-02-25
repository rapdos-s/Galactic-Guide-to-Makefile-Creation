// Importa o cabeçalho da biblioteca unistd, nesse cabeçalho temos o necessário para usar a função unistd
#include <unistd.h>

// int é o retorno, em geral a main retorna 0 quando tudo dá certo ou um outro valor quando um erro ocorre
// main é o nome dado a primeira função que vai ser executada no seu programa, é a porta de entrada dele
// (void) indica que a main não recebe nenhuma informação ou parâmetro externo quando executado
int main(void)
{
    // write é uma função básica para escrever algo em algum lugar, uma alternativa comum nesse caso é usar a função printf()
    // 1: É primeiro parâmetro, indica onde escrever, no caso 1 é a saída padrão do terminal
    // "Hello, World!\n": É o que vai ser escrito
    // 14: É a quantidade de caracteres que serão escrito, note que '\n' (quebra de linha) é considerado como um caractere, por isso 14 e não 15.
    write (1, "Hello, World!\n", 14);
    // return: Indica um valor que vai ser retornado pra quem chamou a função
    // 0: Em geral, significa que tudo deu certo
    return (0);
}
