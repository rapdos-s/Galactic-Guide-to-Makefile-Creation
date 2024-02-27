#### Parte anterior : [Básico 02 - Dependência nas receitas e Variáveis](https://github.com/rapdos-s/Galactic-Guide-to-Makefile-Creation/tree/main/basic%2002%20-%20Recipe%20Dependencies%20and%20Variables)

A partir de agora vamos usar alguns padrões.

Um Makefile simples costuma ter receitas básicas para compilar e limpar um projeto e receitas que auxiliam nesse processo.

Algumas receitas comuns são:

`all` Gera todos os arquivos intermediários e finais;

`clean` Limpa todos os arquivos intermediários;

`fclean` limpa arquivos intermediários e finais;

`re` limpa arquivos intermediários e finais e gera os arquivos intermediários e finais.

Nota:
Talvez tenha ficado confuso os termos intermediários ou finais.

Aqui estamos separando arquivos em três partes:

`Arquivos de origem` são arquivos que serão usados de base para o projeto. Ex.: Códigos, cabeçalhos, etc;

`Arquivos intermediários` são arquivos gerados usando os arquivos de origem, em geral são necessários em algum passo, mas não para a execução. Ex.: Objetos, Dependências, etc;

`Arquivos finais` são os arquivos que, de fato, compoem a saída do projeto como binários que podem ser executados ou bibliotecas. Ex.: .out, .a, .exe, etc;

<h2></h2> <!-- Line divisor -->

Os próximos passos terão muitos conceitos, minha ideia não é explicar C a fundo neste guia, mas focar em Makefile, por isso vou apenas deixar os arquivos de exemplo e falar sobre o que cada um representa sem entrar em muitos detalhes.

Vamos primeiro criar três arquivos de origem:

`main.c` contém a função principal main() que vai chamar a função hello_world();

`hello_world.c` apenas imprime o texto "Hello, World!" na saída padrão;

`hello_world.h` e um arquivo de cabeçalho que main.c usa para poder usar a função hello_world().

Não há boas práticas nos arquivos, eles foram feitos para serem simples e direto. ¯\_(ツ)_/¯

De início vamos definir as receitas básicas, remova qualquer texto do Makefilea tual e coloque o seguinte conteúdo:
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

Agora teremos que usar o que aprendemos até aqui e alguns conhecimentos de como compilar projetos em C para prosseguir, as coisas podem ficar bem complexas, mas não se preocupe, vamos seguir firmes.

#### all

Por definição essa regra gera todos os arquivos finais. Ou seja, ela não faria algo em sí, mas sim chama outras regras que gerarão os arquivos finais, no nosso caso, binários executáveis.

Simulação de um Makefile para um projeto multibinários:

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
```bash
compilando arquivo final um.out
compilando arquivo final dois.out
Todos os arquivos foram gerados
```

Esse exemplo é apenas para entender o papel dele porque isso não estará tão claro em um projeto que gera apenas um binário.

Outra informação importante nesse ponto é que o make possui valores padrões para certos casos, é possível ver isso com a flag `-p`: `make -p`.

É uma lista bem grande. Usar `make -p | grep "CC = "` vai ajudar a filtra o valor, no meu caso é o cc, a saída esperada é:
```bash
YACC = yacc
CC = cc
```
Fiz esse comentário porque certas coisas podem funcionar de uma forma inesperada, muitas vezes "sozinhas" já que algumas variáveis ou receita não foram definidas de forma explícita. Pra esse guia vamos definir todas as variáveis de forma explícita para evitar surpresas.

<h2></h2> <!-- Line divisor -->

Bom, vamos usar por base um comando para compilar um arquivo especificando o arquivo de saída:
```bash
cc *.c -o a.out
```

Pra isso vamos criar variáveis que indicarão cada parte e deixarão mais fácil adicionar arquivos futuramente, alterar o arquivo de saída e configurar o compilador

```make
NAME = a.out
SOURCE = $(wildcard *.c)
CC = cc

$(NAME):
	$(CC) $(SOURCE) -o $(NAME)

```

Como disse antes, all é responsável por gerar todos os arquivos finais, logo ele vai depender da geração de todos eles ou, no nosso caso, ele, já que só temos o $(NAME).

```make
NAME = a.out
SOURCE = $(wildcard *.c)
CC = cc

all: $(NAME)

$(NAME):
	$(CC) $(SOURCE) -o $(NAME)

```

#### clean

O próximo passo seria apagar os arquivos intermediários, mas nesse ponto não estamos fazendo uso deles, portanto, vamos apenas deixar vazio e passar para o próximo ponto

```make
NAME = a.out
SOURCE = $(wildcard *.c)
CC = cc

all: $(NAME)

$(NAME):
	$(CC) $(SOURCE) -o $(NAME)

clean:

```

#### fclean

A receita fclean primeiro faz o mesmo que clean, nesse caso é comum que dependa de clean como uma forma de forçar que clean seja executada, isso é bom porque ainda não definimos clean, mas quando o fizermos, já teremos o fclean pronto.
Após isso, fclean limpa os arquivos finais, no caso $(NAME), pra isso usamos o `rm` com a flag `-f` para forçar que a remoção seja feita sem nenhuma interrupção ou erro caso não encontre o arquivo. Eu costumo definir a variável `$(RM)` pra esse caso.

```make
NAME = a.out
SOURCE = $(wildcard *.c)
CC = cc
RM = rm -f

all: $(NAME)

$(NAME):
	$(CC) $(SOURCE) -o $(NAME)

clean:

fclean: clean
	$(RM) $(NAME)

```

#### re

Por fim, temos o re, uma regra simples de fazer, pois ela faz justamente `fclean` e `all` em sequencia ou seja, é equivalente a `make fclean all`.

```make
NAME = a.out
SOURCE = $(wildcard *.c)
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

Com isso temos um Makefile funcional capaz de compilar um projeto simples.

Para compilar, basta um `make`

Quando precisamos compilar novamente, usamos `make re`

Quando precisamos limpar, usamos `make fclean`

Mas temos um problema, apesar de funcional, esse Makefile não entende quando um projeto é atualizado, isso é fundamental para evitar o relink e facilitar atualizações futuras.

Vamos ver isso na parte seguinte.

<h2></h2> <!-- Line divisor -->

Para fechar essa parte, vamos adicionar um pequeno detalhe: o `.PHONY`.

Ele tem um papel interessante.

Quando uma receita depende de algo, em geral, esse algo está em duas categorias: `arquivos ou diretórios` e `outras receitas`.

Perceba que existem casos em que pode haver dúvidas, uma receita pode ter o mesmo nome de um arquivo ou diretório.

Por exemplo, se tivermos esse Makefile:
```make
test:
	@echo teste
```

Ao executar `make test` teremos a saída:
```bash
test
```

Se criarmos um arquivo chamado `test` na mesma pasta e tentarmos executar `make test` teremos:
```bash
make: 'test' is up to date.
```

Ele entende que nada precisa ser feito pois o arquivo já existe e está atualizado.

Nesse caso usamos o `.PHONY` para indicar que `test`, nesse caso, é a receita, e não o arquivo:
```bash
test:
	@echo teste

.PHONY: test
```

Ao executar `make test` teremos novamente a saída:
```bash
test
```

Aqui vale uma nota: O Makefile considera algumas receitas como especiais e já trata elas ignorando arquivos, por segurança eu sempre separo o que é arquivo do que é a receita.

Nos vemos na próxima parte o/

#### [Básico 04 - Finalizando e removendo relink](https://github.com/rapdos-s/Galactic-Guide-to-Makefile-Creation/tree/main/basic%2004%20-%20Removing%20relink%20finishing)

### Quer apoiar o conteúdo?
Caso queira fazer isso via pix, basta escanear o qrcode

<img src="../assets/qrcode-pix.png" alt="QRCode Pix" width="300"/>
