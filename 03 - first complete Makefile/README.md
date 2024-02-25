A partir de agora vamos usar alguns padrões.

Um Makefile simples costuma ter receitas básicas para compilar e limpar um projeto e receitas que auxiliam nesse processo.
Algumas receitas comuns são:

all: Gera todos os arquivos intermediários e finais
clean: Limpa todos os arquivos intermediários.
fclean: limpa arquivos intermediários e finais
re: limpa arquivos intermediários e finais e gera os arquivos intermediários e finais

Nota:
Talvez tenha ficado confuso os termos intermediários ou finais.
Aqui estamos separando arquivos em três partes:
Arquivos de origem: São arquivos que serão usados de base para o projeto. Ex.: Códigos, cabeçalhos, etc.
Arquivos intermediários: São arquivos gerados usando os arquivos de origem, em geral são necessários em algum passo, mas não para a execução. Ex.: Objetos, Dependências, etc.
Arquivos finais: São os arquivos que, de fato, compoem a saída do projeto como binários que podem ser executados ou bibliotecas. Ex.: .out, .a, .exe, etc

Os próximos passos terão muitos conceitos, minha ideia não é explicar C a fundo neste guia, mas focar em Makefile, por isso vou apenas deixar os arquivos de exemplo e falar cobre o que cada um representa sem entrar em muitos detalhes.

Vamos primeiro criar três arquivos de origem:
main.c: Contém a função principal main() que vai chamar a função hello_world();
hello_world.c: Apenas imprime o texto "Hello, World!" na saída padrão
hello_world.h: E um arquivo de cabeçalho main.c usa para poder usar a função hello_world().

Note que não há boas práticas nos arquivos, eles foram feitos para serem simples e direto

De início vamos definir as receitas básicas:

```make
all:
	@echo Compila o projeto

clean:
	@echo Limpa arquivos intermediários

fclean:
	@echo limpa arquivos intermediários e finais

re:
	@echo limpa arquivos intermediários e finais e compila o projeto
```

Agora teremos que usar o que aprendemos até aqui e alguns conhecimentos de como compilar projeots em C para prosseguir, as coisas podem ficar bem complexas, vamos então seguir firme.

ALL

Por definição essa regra gera todos os arquivos finais. Ou seja, ela não faria algo em sí, mas sim chama outras regras que gerarão os arquivos finais, no nosso caso binários executáveis.

Exemplo de um Makefile para um projeto multibinários, não é real, apenas para simular:

```make
NAME_UM = um.out
NAME_DOIS = dois.out

all: $(NAME_UM) $(NAME_DOIS)
	@echo "Todos os arquivos foram gerados"

$(NAME_UM):
	@echo "compilando arquivo final $(NAME_UM)"

$(NAME_DOIS):
	@echo "compilando arquivo final $(NAME_DOIS)"

```

Saída esperada:
```
make
compilando arquivo final um.out
compilando arquivo final dois.out
Todos os arquivos foram gerados
```

Veja que all tem esse nome porque é responsável por gerar todos os binários.

Esse exemplo é apenas para entender o papel dele porque isso não estará tão claro a seguir.

Outra informação importante nesse ponto é que o make possui valores padrões para certos casos, é possível ver isso com o comando make -p (É uma lista bem grande). make -p | grep "CC = " vai ser possível ver o valor, no meu caso é o cc.


```
make -p | grep "CC = "
YACC = yacc
CC = cc
```
Digo isso porque certas coisas podem funcionar de uma forma inesperada porque alguma variável ou receita não foi definida da forma correta. Pra esse guia vamos definir todas as variáveis de forma explícita para evitar surpresas.

Bom, vamos usar por base um comando para compilar um arquivo especificando o arquivo de saída:
```
cc main.c hello_world.c -o a.out
```

Pra isso vamos criar variáveis que indicarão cada parte e deixarão mais fácil adicionar arquivos futuramente, alterar o arquivo de saída e configurar o compilador

```
NAME = a.out
SOURCE = main.c hello_world.c
CC = cc

$(NAME):
	$(CC) $(SOURCE) -o $(NAME)

```

Como disse antes, all é responsável por gerar todos os arquivos, logo ele vai depender da geração de todos eles ou, no nosso caso, ele, já que só temos o $(NAME).

```
NAME = a.out
SOURCE = main.c hello_world.c
CC = cc

all: $(NAME)

$(NAME):
	$(CC) $(SOURCE) -o $(NAME)

```

CLEAN

O próximo passo seria apagar os arquivos intermediários, mas nesse ponto não estamos fazendo uso deles. Nesse caso vamos apenas deixar vazio e passar para o próximo ponto

```
NAME = a.out
SOURCE = main.c hello_world.c
CC = cc

all: $(NAME)

$(NAME):
	$(CC) $(SOURCE) -o $(NAME)

clean:

```

FCLEAN

A receita fclean primeiro faz o mesmo que clean, nesse caso é comum que dependa de clean como uma forma de forçar que clena seja executada, isso é bom porque ainda não definimos clean, mas quando o fizermos, já teremos o fclean pronto.
Em sgundo, fclean limpa os arquivos finais, no caso $(NAME), pra isso usamos o rm com a flag -f para forçar que a remoção seja feita sem nenhuma interrupção. Eu costumo definir a variável RM pra esse caso.

```
NAME = a.out
SOURCE = main.c hello_world.c
CC = cc
RM = rm -f

all: $(NAME)

$(NAME):
	$(CC) $(SOURCE) -o $(NAME)

clean:

fclean: clean
	$(RM) $(NAME)

```

RE

Por fim, temos o re, uma regra simples de fazer, pois ela faz justamente fclean e all em sequencia ou seja, é equivalente a make fclean all.

```
NAME = a.out
SOURCE = main.c hello_world.c
CC = cc
RM = rm -f

all: $(NAME)

$(NAME):
	$(CC) $(SOURCE) -o $(NAME)

clean:

fclean: clean
	$(RM) $(NAME)

re: fclean all

```

Com isso temos um Makefile funcional capáz de compilar um projeto simples.
Para compilar, basta um make
Quando precisamos compilar novamente, usamos make re
Quando precisamos limpar, usamos make fclean

Mas temos um problema, apesar de funcional, esse Makefile não entende quando um projeto é atualizado, isso é fundamental para evitar o relink e facilitar atualizações futuras.

Vamos ver isso na parte seguinte.