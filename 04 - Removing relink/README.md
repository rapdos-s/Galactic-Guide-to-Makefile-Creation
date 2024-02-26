Chegamos em um ponto onde teremos que entender algumas ferramentas que o make e que o compilador (no nosso caso, o cc) usam para gerar os arquivos intermediários e finalizar o clean enquanto evitamos o relink.

Alguns pontos importantes:

ARquivos intermediários

.o (objetos): São arquivos que são compilados, eles não são arquivos finais e não necessariamente possuem uma função main. É importante ter esses arquivos porque econimiza tempo caso você já tenha algo compilado que não precise ser compilado novamente.
Por exemplo, se você alterou o arquivo main.c, mas não o hello_world.c, main será compilado em um main.o, hello_world será ignorado, pois já está atualizado e tudo será compilado em a.out, pois main.o foi atualizado.

.d (dependencias): São bem interessantes. Eles são, va verdade receitas, mas sem comandos, a ideia deles é que você tenha uma descrição de qual arquivo depende de qual arquivo sem precisar especificar isso manualmente.
Por exemplo, helllo_world.o é um obejto que depende de hello_world.c e hello_world.h para ser gerado, logo ele vai verificar se eles foram atualizados antes de tentar compilar.

Ok, mas como eles sabem oq ue está atualizado ou não?
Usam a data (timestamp) do arquivo e comparam, se algo possui uma dependencia que é mais nova, esse algo precisa ser atualizado.

Bom, na prática, o que precisamos?
Primeiro, vamos usar objetos e depencencias, nesse caso é necessário informar quais os nomes dos arquivos.
Em geral, arquivos .o (obejtos) possuem o mesmo nome de arquivos .c, só trocando a extensão.
O mesmo vale para arquivos .d (dependencias), porém com a extensão .d.

Para isso vamos criar as variáveis OBJECTS e DEPENDENCIES, nos dois casos, vamos usar o valor de SOURCE.
```
SOURCE = main.c hello_world.c
OBJECTS = $(SOURCE)
DEPENDENCIES = $(SOURCE)

test:
	@echo SOURCE = $(SOURCE)
	@echo OBJECTS = $(OBJECTS)
	@echo DEPENDENCIES = $(DEPENDENCIES)
```

SAída esperada:
```
make test
SOURCE = main.c hello_world.c      
OBJECTS = main.c hello_world.c     
DEPENDENCIES = main.c hello_world.c
```

Para alterar a extenção, vamos ter que relembrara que variáveis em Makefiles são strings ou arrays de strings separadas por espeças, nesse caso é possível fazer alterações em cada string no array:
```
OBJECTS = $(SOURCE:%.c=%.o)
```
Isso significa que pegamos os valores de SOURCE, porém para cada valor vamos alterar o final de cada string de ".c" para ".o" mantendo o início usando "%".

```
SOURCE = main.c hello_world.c
OBJECTS = $(SOURCE:%.c=%.o)
DEPENDENCIES = $(SOURCE:%.c=%.d)

test:
	@echo SOURCE = $(SOURCE)
	@echo OBJECTS = $(OBJECTS)
	@echo DEPENDENCIES = $(DEPENDENCIES)
```

SAída esperada:
```
make test
SOURCE = main.c hello_world.c
OBJECTS = main.o hello_world.o
DEPENDENCIES = main.d hello_world.d
```

Nota: Atenção a detalhes como espaços, a sintaxe pode ser bem sensível em alguns casos.

Bom, agora já sabemos os nomes dos arquivos de objetos e dependencias, precisamos de alguns passos, por isso vamos em frente.

Pra começar, temos que deixar de gerar o arquivo final a partir do arquivo .c e sim, gerar em duas etapas, primeiro gerar os arquivos .o, depois gerar $(NAME)

o make usa receita pronta apra isso(.o:.c ou %.o:%.c), elas são padrões e suficientes, mas é bom definiar sua própria para o caso de precisar usar alguma configuração diferente da padrão.
```
%.o: %.c
	$(CC) -c $< -o $@
```

Ok, agora as coisas ficaram estranhas, mas podemos explicar cada ponto:
%.o: Significa que serve para qualquer arquivos que termine com .o
%.c: Significa que o arquivo .o depende de um arquivo de igual nome, mas com a extensão .c
$(CC): É a variável que diz qual compilador será usado
-c: Significa que o arquivo que vai ser compilado pode ou não ter uma função main, normlamente só um arquivo possui essa função no projeto, os outros normalmente não possuem.
$<: É um símbolo especial que indica o valor da primeira dependência, no caso o arquivo .c que será compilado, precisamos fazer dessa forma para o make entender que deve usar o valor apenas do arquivo atual ($(SOURCE) usaria todos os arquivos)
-o indica que o parâmetro seguinte é o nome de saída do arquivo, nesse caso o objeto
$@ é um símbolo especial semelhante ao $<, porém esse se refere ao valor da receita, ou seja, o nome do arquivo de objeto.

Simplificando:

```
main.o: main.c
	cc -c main.c -o main.o

hello_world.o: hello_world.c
	cc -c hello_world.c -o hello_world.o
```

Veja que seria possível criar uma receita para cada arquivo no seu projeto, mas fazendo assim, conseguimos simplificar projetos maiores e facilitamos quando é necessário adicionar um novo arquivo.

Agora precisamos informar que precisamos gerar os arquivos de objetos e usar esses arquivos no momento adequado. Fazemos isso na hora de compilar o binário final, no nosso caso o $(NAME). Ele deve depender da criação de objetos, assim a regra que acabamos de criar vai ser chamada para cada objeto antes de compilar o binário:

```
$(NAME): $(OBJECTS)
	$(CC) $(SOURCE) -o $(NAME)
```

A saída esperada:

```
make re
rm -f a.out
cc -c main.c -o main.o
cc -c hello_world.c -o hello_world.o
cc main.c hello_world.c -o a.out
```

O legal aqui é que podemos adicionar novo arquivos em SOURCE conforme o projeto cresce sem precisar criar regras para cada arquivo. Mas ainda não estamos usando os arquivos de objeto na compilação final, pra isso vamos deixar de usar os arquivos de $(SOURCE) e usaremos os arquivos de $(OBJECTS):

```
$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(NAME)
```

Vou primeiro remover os objetos (spoiler da receita clean :eyes:)
```
rm -f *.o
```

Saída esperada:
```
make re
rm -f a.out
cc -c main.c -o main.o
cc -c hello_world.c -o hello_world.o
cc main.o hello_world.o -o a.out
```

Testando:
```
./a.out
Hello, World!
```

Agora, caso você altere algo, por exemplo, duplicando a chamada da função no arquivo main.c

````
#include "hello_world.h"

int main(){
    hello_world();
    hello_world();
}

```

A saída esperada do make em seguida será:
```
make
cc -c main.c -o main.o
cc main.o hello_world.o -o a.out
```

Note que hello_world.o não foi compilado novamente, isso porque nem hello_world.c, nem hello_world.h não foram alterado.
Como main.o depende de main.c, ele foi compilado novamente.
Como a.out depende de main.o ele também foi compilado novamente.

DEPENDENCIAS

Mas espera, em nenhum momento indicamos que alguém depende do arquivo hello_world.h, o que acontece se alterarmos ele?
Antes vamos tentar um make sem nenhuma alteração:

```
make
make: Nothing to be done for 'all'.
```

Essa mensagem indica que nada precisa ser feito para satisfazer all, ou seja, tudo está como deveria.

Vou adicionar um comentário no arquivo hello_world.h, note que isso não altera em nada a execução, mas atualiza o timestamp do arquivo, forçando o make a atualizar os arquivos que dependem dele:

```
#include <unistd.h>

// Imprime "Hello, World!" seguido de uma quebra de linha
void hello_world();

```

TEntamos usar o make novamente e temos o seguinte:

```
make
make: Nothing to be done for 'all'.
```

Ou seja, o Makefile não entende ainda que os arquivos main.c e hello_world.c dependem de hello_world.h. Para corrigir isso podemos fazer na mão criando regras para cada arquivo:

```
main.o: main.c hello_world.o hello_world.h
hello_world.o: hello_world.c hello_world.h
```

Note que nem precisamos detalhar como a compilação vai acontecer como fazemos em `%.o: %.c` ou em `$(NAME): $(OBJECTS)`, pois o make entende que é um padrão de um arquivo .o que depende de um arquivo .c de mesmo nome e usa os mesmos comandos.

Ok, mas listar qual arquivo .o depende de quais outros arquivos na mão é algo que pode se transformar em um incovêniente bem rapidamente em um projeto grande. Principalmente quando pensamos que não é um padrão. main. o depende de três aqruivos enquanto hello_world.o depende de apenas dois, esquecer algo mais pra frente pode ser um problema.

Pra resolver isso, temos duas ferramentas que trabalham bem juntas, a primeira é a capacidade de importar arquivos para dentro do Makefile e usa-los como parte do próprio Makefile, vejamos:

Vamos adicionar um comando de include no Makefile que fará isso, note que ele usa uma variável que não existe:

```
-include duck.mk

duck:
	@echo duck: $(QUACK)
```

A saída esperada:
```
make duck
duck:
```

O que podemos fazer é criar o arquivo duck.mk e inserir a variável dentro dele, ficando:

```
QUACK = "furious quacking sound effect!"
```

TEstando novamente teremos:
```
make duck
duck: furious quacking sound effect!
```

Podemos importar includive a receita, no caso o arquivo duck.mk ficaria:

```
QUACK = "furious quacking sound effect!"

duck:
	@echo duck: $(QUACK)

```

O que aconteceu aqui é que a linha -include foi substituida pelo conteúdo do arquivo duck.mk. Isso possui várias implicações.
Uma delas está no comando make sem nenhuma receita sendo passada com parâmetro, veja que adiconamos o include quase no final, depois de já termos várias receitas, isso significa que a primeira será chamada (all), mas caso coloquemos o include no início teremos:

```
-include duck.mk

NAME = a.out

SOURCE = main.c hello_world.c
OBJECTS = $(SOURCE:%.c=%.o)
DEPENDENCIES = $(SOURCE:%.c=%.d)

CC = cc
RM = rm -f

all: $(NAME)

clean:

fclean: clean
	$(RM) $(NAME)

re: fclean all

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(NAME)

%.o: %.c
	$(CC) -c $< -o $@

main.o: main.c hello_world.o hello_world.h
hello_world.o: hello_world.c hello_world.h

duck:
	@echo duck: $(QUACK)

```

Ao tentar usar o make sem nenhuma receita:

```
make
duck: furious quacking sound effect!
```

Isso pode ser um problema, já que muitas vezes queremos garantir que `make` sempre use uma regra específica.
Para resolver isso, vamos usar a variável especial .DEFAULT_GOAL, ela vai indicar qual a receita do Makefile o make deve usar por padrão, basta dar o valor all:

.DEFAULT_GOAL = all

Ok, mas vamos deixar duck.mk de lado e focar em incluir arquivos de dependência.

Prar incluir esse arquivos, primeiros precisamos saber quais são eles, já fizemos isso:

```
DEPENDENCIES = $(SOURCE:%.c=%.d)
```

Agora que sabemos quais são, precisamos incluir esses arquivos:

```
-include $(DEPENDENCIES)
```

PAra evitar qualquer problema, vamos remover as receitas que criamos na mão:

```
main.o: main.c hello_world.o hello_world.h
hello_world.o: hello_world.c hello_world.h
```

Por fim, vamos gerar esse arquivos na hora da compilação, pra isso vamos usar um recurso do compilador, no momento fazemos algo assim:

```
cc -c codigo.c -o objeto.o
```

O -o aí tá indicando um arquivo de saída que é um obejto com o código compilado. Para gerar um arquivo de dependencia usamos a flag -MMD, o que ficaria:

```
cc -c codigo.c -o objeto.o -MMD
```

Nota: -MMD significa que um arquivo de dependencia deve ser gerado no processo com uma receita (da mesma forma que fizemos antes na mão), o nome desse arquivo é baseado no nome do arquivo que é indicado em `-o`.
Por isso $(DEPENDENCIES) usa o `%.o` e não o `%.c` para gerar o nome dos arquivos `%.d`, já que os arquivos de objeots podem ficar em locais diferentes de arquivos de origem, mas isso é assunto pra outra hora. Spoiler: Caso queira especificar o nome do arquivo, use -MF <nome do arquivo> logo após -MMD.
NOta: Você pode usar -MD caso queira que arquivos de sistema também serão levados em consideração, mas aqui vamos focar somente nos seus arquivos de projeto.
Para mais informações, consulte usando o comando `man cc` ou (recomendo) no site man7.org (https://man7.org/linux/man-pages/man1/gcc.1.html)

Enfim, fica assim:

```
%.o: %.c
	$(CC) -c $< -o $@ -MMD
```

Nesse ponto, seria muito bom fazer algo que está incompleto, a receita clean. Ela precisa remover os arquivos intermediários, no caso desse projeto os objetos e dependências, isso fica simples agora:

Nota: Cuidado aqui, pois estaremos removendo arquivos, tenha certeza de que está removendo os arquivos certos, um teste simples pode ajudar:
```
test:
	@echo DEPENDENCIES = $(DEPENDENCIES)
	@echo OBJECTS = $(OBJECTS)
```
Você também pode usar a flag -n, ela apenas mostra os comandos que serão executados sem de fato executar eles:
```
make clean -n
rm -f main.d hello_world.d
rm -f main.o hello_world.o
```
ou
```
make -n clean
rm -f main.d hello_world.d
rm -f main.o hello_world.o
```

```
clean:
	$(RM) $(DEPENDENCIES) $(OBJECTS)
```
Saída esperada:
```
make clean
rm -f main.d hello_world.d main.o hello_world.o
```

ou

```
clean:
	$(RM) $(DEPENDENCIES)
	$(RM) $(OBJECTS)
```
Saída esperada:
```
make clean
rm -f main.d hello_world.d
rm -f main.o hello_world.o
```

Com isso temos um Makefile que sabe o que e quando atualizar em um projeto simples.
Você pode adicionar novos arquivos .c em SOURCE.
Alterar o nome de saída alterando $(NAME).
Mudar configurações como o compilador em $(CC) ou alterar $(RM) para funcionar em pastas com a flag -r.
É possível adicionar saídas mais descritivas com echo e ocultar comandos com `@`.

Muitas coisas podem ser feitas usando esse Makefile de base, futuramente teremos adendos aqui para deixar o Makefile mais robusto ou adaptar para outras situações.