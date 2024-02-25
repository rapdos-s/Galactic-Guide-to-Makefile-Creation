# first-Makefile
Simples guia de como criar seu primeiro Makefile para um projeto em C


O que você precisa?
Editor de texto
    Basicamente qualquer editor de texto funcionará, bloco de notas, notepad++, vim, VSCode, etc.
Make
    Para verificar se está instalado, use o comando 'make --version'  no seu cmd, power shell, bash ou qualquer equivalente, para facilitar, usarei comandos do terminal padrão do Linux Ubuntu.
Compilador
    Aqui vai depender um pouco de qual vai escolhers, em geral cc (cc --version), gcc (gcc --version) ou algum equivalente. No Makefile você irá definir qual será utilizado.

Navegue até o diretório de seu projeto e crie um arquivo chamado Makefile

Para manter as coisas simples, apenas copie o código abaixo para dentro do arquivo Makefile:

```make
test:
	echo "Hello, Makefile!"
```
Nota: Repare que antes de ´@echo´ temos um tab e não espaços.

No terminal, estando no diretório do seu Makefile, execute o comando:
```
make
```

A saída esperada é:
```
echo "Hello, Makefile!"
Hello, Makefile!
```

Se está foi a saída, significa que você já tem o suficiente para executar comandos dentro do Makefile e podemos entender melhor o que está acontecendo.

Basicamente criamos uma forma de chamar o comando echo para imprimir "Hello, Makefile!", explicando cada elemento, temos:

`test` o nome da regra, receita ou alvo, nesse guia chamaremos de receita
`:` separa a receita das dependências (Falaremos disso depois)
`echo "Hello, Makefile!"` é um comando simples para mostrar "Hello, Makefile!" no terminal.

Nomeando tudo ficaria:
```make
receita:
	comando
```

Podemos colocar mais de um comando em uma receita:
```
test:
	echo "Este é o primeiro comando"
	echo "Este é o segundo comando"
```

A saída esperada é:
```make
make
echo "Este é o primeiro comando"
Este é o primeiro comando
echo "Este é o segundo comando" 
Este é o segundo comando
```

Vemos que os dois comandos foram executados, mas está um pouco poluído, podemos usar `@` para que o make não mostre o comando que está sendo executado, mas devemos ter cuidado, pois isso pode ocultar informações importantes mais a frente.
```
test:
	@echo "Este é o primeiro comando"
	@echo "Este é o segundo comando"
```

A saída esperada é:
```make
make
Este é o primeiro comando
Este é o segundo comando
```

Agora que temos uma receita com mais de um comando, podemos criar mais de uma receita:

```
um:
	@echo "PRIMEIRO comando da receita UM"
	@echo "SEGUNDO comando da receita UM"

dois:
	@echo "PRIMEIRO comando da receita DOIS"
	@echo "SEGUNDO comando da receita DOIS"
```

A saída esperada é:
```make
make
PRIMEIRO comando da receita UM
SEGUNDO comando da receita UM
```

Podemos ver que executamos os comandos da primeira receita, mas a segunda não foi executada.
Isso acontece porque o make executa por padrão a primeira receita.

Para escolher quais receitas devem ser executadas, devemos expecificar no comando make dessa forma:
make <receita_um> <receita_dois> ... <receita_n>

Para ver isso na prática podemos fazer alguns testes:

executando a receita padrão:

A saída esperada é: make
```make
make
PRIMEIRO comando da receita UM
SEGUNDO comando da receita UM
```

executando a primeira receita: make um

A saída esperada é:
```make
make um
PRIMEIRO comando da receita UM
SEGUNDO comando da receita UM
```

executando a segunda receita: make dois

A saída esperada é:
```make
make dois
PRIMEIRO comando da receita DOIS
SEGUNDO comando da receita DOIS
```

executando mais de uma receita: make um dois

A saída esperada é:
```make
make um dois
PRIMEIRO comando da receita UM  
SEGUNDO comando da receita UM   
PRIMEIRO comando da receita DOIS
SEGUNDO comando da receita DOIS
```

Com isso já podemos preparar comandos para compilar e executar códigos, para facilitar vamos criar um "Hello, World!" em C:

Criamos um arquivo main.c no mesmo diretório que o Makefile:
```c
#include <unistd.h>

int main(void)
{
    write (1, "Hello, World!\n", 14);
    return (0);
}

```

A seguir temos comentários com uma breve explicação sobre o código, não é obrigatório entender agora, por isso pode pular se quiser focar no no Makefile.

```c
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

```

Um comando simple para compilar esse arquivo seria
```sh
cc main.c
```
Isso pode varia dependendo do compilador que está usando, tenha isso em mente.

Para fazer o make executar esse comando, podemos criar uma receita e chamar ela:
```make
compilar:
	cc main.c
```

A saída esperada:
```sh
make
cc main.c
```

Também é esperado que um arquivo apareça no mesmo diretório, normalmente chamado `a.out`
Para executar o código usamos `./a.out ` no terminal

A saída esperada:
```sh
./a.out 
Hello, World!
```

Se tudo deu certo até aqui, temos uma receita que compila o código, mas se quisermos remover o arquivo compilado ou executa-lo, teremos que fazer na mão.

Pra isso vamos criar novas receitas

```make
compilar:
	cc main.c

apagar:
	rm a.out

executar:
	./a.out
```

Nota: Apagar arquivos por engano é bem comum, tome o cuidado de fazer backup ou manter tudo num repositório git

As saídas esperadas:

Compilação
```sh
make
cc main.c
```

Execução
```sh
make executar
./a.out
Hello, World!
```

Remoção
```sh
make apagar
rm a.out
```

Com isso já conseguimos o básico, quase tudo que fazemos num terminal é possível fazer dentro do Makefile, mas existem duas coisas muito importantes num Makefile que farão toda diferença: Dependências e Variáveis.

Eu pessoalmente já consideraria isso como algo intermediário, por isso 
