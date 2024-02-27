#### Parte anterior : [Básico 03 - Escrevendo um Makefile Básico](https://github.com/rapdos-s/Galactic-Guide-to-Makefile-Creation/tree/main/basic%2003%20-%20Writing%20a%20basic%20Makefile)

Chegamos em um ponto onde teremos que entender algumas ferramentas que o make e o compilador (no nosso caso, o cc) usam para gerar os arquivos intermediários e finalizar o clean enquanto evitamos o relink.

<h2></h2> <!-- Line divisor -->

Alguns pontos importantes:

#### Arquivos intermediários

`.o (objetos)` são arquivos que são compilados, não são arquivos finais e não, necessariamente, possuem uma função main. É importante ter esses arquivos porque economiza tempo caso você já tenha algo compilado que não precise ser compilado novamente.

Por exemplo, se você alterou o arquivo main.c, mas não o hello_world.c, main.c será compilado em um main.o, hello_world.c será ignorado, pois hello_world.o já está atualizado e tudo será compilado em a.out, pois main.o foi atualizado.

`.d (dependencias)` são bem interessantes. Eles são receitas, mas não possuem comandos. A ideia deles é que você tenha uma descrição de qual arquivo depende de qual arquivo sem precisar especificar isso manualmente.

Por exemplo, hello_world.o é um obejto que depende de hello_world.c e hello_world.h para ser gerado, logo ele vai verificar se eles foram atualizados antes de tentar compilar.

Ok, mas como eles sabem o que está atualizado ou não?
Usam a data e hora (timestamp) do arquivo e comparam, se algo possui uma dependencia que é mais nova, esse algo precisa ser atualizado.

<h2></h2> <!-- Line divisor -->

Bom, na prática, o que precisamos?
Primeiro, vamos saber quais são os objetos, nesse caso é necessário informar quais os nomes dos arquivos.
Em geral, arquivos .o (objetos) possuem o mesmo nome de arquivos .c, só trocando a extensão.

Para isso vamos criar as variáveis `$(OBJECTS)`, nos dois casos, vamos usar o valor de `$(SOURCE)`.
```make
SOURCE = main.c hello_world.c
OBJECTS = $(SOURCE)

test:
	@echo SOURCE = $(SOURCE)
	@echo OBJECTS = $(OBJECTS)
```

Executando `make test` a saída esperada será :
```
SOURCE = main.c hello_world.c      
OBJECTS = main.c hello_world.c     
```

Para alterar a extensão, vamos ter que relembrar que variáveis em Makefiles são strings ou arrays de strings separadas por espaços.

No arquivo Makefile é possível fazer alterações em cada string no array:
```make
OBJECTS = $(SOURCE:%.c=%.o)
```
Isso significa que pegamos os valores de `$(SOURCE)`, porém para cada valor vamos alterar o final de cada string de ".c" para ".o" mantendo o que vem antes do `.`.

```
SOURCE = main.c hello_world.c
OBJECTS = $(SOURCE:%.c=%.o)

test:
	@echo SOURCE = $(SOURCE)
	@echo OBJECTS = $(OBJECTS)
```

Executando `make test` a saída esperada será :
```
SOURCE = main.c hello_world.c
OBJECTS = main.o hello_world.o
```

Nota: Atenção a detalhes como espaços, a sintaxe pode ser bem sensível em alguns casos.

<h2></h2> <!-- Line divisor -->

Bom, agora já sabemos os nomes dos arquivos de objetos, é um ótimo começo.

Agora temos que deixar de gerar o arquivo final a partir do arquivo .c e sim, gerar em duas etapas, primeiro gerar os arquivos .o, depois gerar $(NAME)

O make usa receitas prontas para isso (`.o:.c` ou `%.o:%.c` no caso da linguagem C), elas são padrões e suficientes, mas é bom definir suas próprias para o caso de precisar usar alguma configuração diferente da padrão.
```make
.o: .c
	$(CC) -c $< -o $@
```
ou
```make
%.o: %.c
	$(CC) -c $< -o $@
```
Ok, agora as coisas ficaram estranhas, mas podemos explicar cada ponto:

`%.o` significa que serve para qualquer arquivos que termine com .o;

`%.c` significa que o arquivo .o depende de um arquivo de igual nome, mas com a extensão .c;

`$(CC)` é a variável que diz qual compilador será usado;

`-c` Significa que o arquivo que vai ser compilado pode ou não ter uma função main, normalmente só um arquivo possui essa função no projeto, os outros normalmente não possuem;

`$<` É um símbolo especial que indica o valor da primeira dependência, no caso o arquivo .c que será compilado, precisamos fazer dessa forma para o make entender que deve usar o valor apenas do arquivo atual (`$(SOURCE)` usaria todos os arquivos);

`-o` indica que o parâmetro seguinte é o nome de saída do arquivo, nesse caso o objeto;

`$@` é um símbolo especial semelhante ao $<, porém esse se refere ao valor da receita, ou seja, o nome do arquivo de objeto.

Simplificando:

```make
main.o: main.c
	cc -c main.c -o main.o

hello_world.o: hello_world.c
	cc -c hello_world.c -o hello_world.o
```

Veja que seria possível criar uma receita para cada arquivo no seu projeto, mas fazendo assim, conseguimos simplificar projetos maiores e facilitamos quando é necessário adicionar um novo arquivo.

Agora precisamos informar que precisamos gerar os arquivos de objetos e usar esses arquivos no momento adequado. Fazemos isso na hora de compilar o binário final, no nosso caso o $(NAME). Ele deve depender da criação de objetos, assim a regra que acabamos de criar vai ser chamada para cada objeto antes de compilar o binário:

```make
$(NAME): $(OBJECTS)
	$(CC) $(SOURCE) -o $(NAME)
```
Executando o comando `make re` a saída esperada será:
```bash
rm -f a.out
cc -c main.c -o main.o
cc -c hello_world.c -o hello_world.o
cc main.c hello_world.c -o a.out
```

Uma coisa legal aqui é que podemos (caso estejamos fazendo manualmente, sem `wildcard`) adicionar arquivos em $(SOURCE) conforme o projeto cresce sem precisar criar novas receitas. Mas ainda não estamos usando os arquivos de objeto na compilação final, pra isso vamos deixar de usar os arquivos de $(SOURCE) e usaremos os arquivos de $(OBJECTS):

```make
$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(NAME)
```

Vamos primeiro remover os objetos (spoiler da receita clean :eyes:)
```bash
rm -f *.o
```

Ao executar `make re`, a saída esperada será:
```bash
rm -f a.out
cc -c main.c -o main.o
cc -c hello_world.c -o hello_world.o
cc main.o hello_world.o -o a.out
```

Ao executar `./a.out`, a saída esperada será:
```bash
Hello, World!
```

Agora, caso você altere algo, por exemplo, duplicando a chamada da função no arquivo main.c
```c
#include "hello_world.h"

int main(){
    hello_world();
    hello_world();
}
```

Ao executar `make`, a saída esperada será:
```bash
cc -c main.c -o main.o
cc main.o hello_world.o -o a.out
```

Note que hello_world.o não foi compilado novamente, isso porque hello_world.c não foi alterado.
Como main.o depende de main.c, ele foi compilado novamente.
Como a.out depende de main.o ele também foi compilado novamente.

<h2></h2> <!-- Line divisor -->

#### Dependências

Mas espera, em nenhum momento indicamos que alguém depende do arquivo hello_world.h, o que acontece se alterarmos ele?

Antes vamos tentar um `make` sem nenhuma alteração:
```bash
make: Nothing to be done for 'all'.
```

Essa mensagem indica que nada precisa ser feito para satisfazer all, ou seja, tudo está como deveria.

Vou adicionar um comentário no arquivo hello_world.h, note que isso não altera em nada a execução, mas atualiza o timestamp do arquivo, forçando o make a atualizar os arquivos que dependem dele:
```c
#include <unistd.h>

// Imprime "Hello, World!" seguido de uma quebra de linha
void hello_world();
```

Tentamos usar o `make` novamente e temos o seguinte:
```bash
make: Nothing to be done for 'all'.
```

Ou seja, o Makefile não entende ainda que os arquivos main.c e hello_world.c dependem de hello_world.h. Para corrigir isso podemos fazer na mão criando regras para cada arquivo:

```make
main.o: main.c hello_world.o hello_world.h
hello_world.o: hello_world.c hello_world.h
```

Note que nem precisamos detalhar como a compilação vai acontecer como fazemos em `%.o: %.c` ou em `$(NAME): $(OBJECTS)`, pois o make entende que é um padrão de um arquivo .o que depende de um arquivo .c de mesmo nome e usa os mesmos comandos.

Ok, mas listar qual arquivo .o depende de quais outros arquivos na mão é algo que pode se transformar em um incovêniente bem rapidamente em um projeto grande. Principalmente quando pensamos que não é um padrão. main.o depende de três arquivos enquanto hello_world.o depende de apenas dois, esquecer algo mais pra frente pode ser um problema.

Pra resolver isso, temos duas ferramentas que trabalham bem juntas, a primeira é a capacidade de importar arquivos para dentro do Makefile e usa-los como parte do próprio Makefile, vejamos:

Vamos adicionar um comando de include no Makefile que fará isso, note que ele usa uma variável que não existe:

```make
-include duck.mk

duck:
	@echo duck: $(QUACK)
```

Ao executar `make duck`, a saída esperada será:
```bash
duck:
```

O que podemos fazer é criar o arquivo duck.mk e inserir a variável dentro dele, ficando:
```make
QUACK = "furious quacking sound effect!"
```

Ao executar `make duck` novamente, a saída esperada será:
```bash
duck: furious quacking sound effect!
```

Podemos importar includive a receita, no caso o arquivo duck.mk ficaria:
```make
QUACK = "furious quacking sound effect!"

duck:
	@echo duck: $(QUACK)
```

O que aconteceu aqui é que a linha `-include duck.mk` foi substituida pelo conteúdo do arquivo duck.mk. Isso possui várias implicações.

Uma delas está no comando make sem nenhuma receita sendo passada com parâmetro, veja que adicionamos o include quase no final, depois de já termos várias receitas, isso significa que a primeira receita que será chamada seja all, mas isso pode mudar se colocar-mos o include no início:

```make
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

Ao executar `make` novamente, a saída esperada, mas não intencional, será:
```
duck: furious quacking sound effect!
```

Isso pode ser um problema, já que muitas vezes queremos garantir que `make` sempre use uma regra específica.
Para resolver isso, vamos usar a variável especial `.DEFAULT_GOAL`, ela vai indicar qual a receita do Makefile o make deve usar por padrão, basta dar o valor all:
```make
.DEFAULT_GOAL = all
```
Ok, mas vamos deixar duck.mk de lado e focar em incluir arquivos de dependência.

<h2></h2> <!-- Line divisor -->

Prar incluir esse arquivos, primeiro precisamos saber quais são eles, já fizemos isso com objetos, agora podemos fazer para dependências criando a variável `$(DEPENDENCIES)`:

```make
DEPENDENCIES = $(OBJECTS:%.o=%.d)
```
Agora que sabemos quais são, precisamos incluir esses arquivos:
```make
-include $(DEPENDENCIES)
```
Para evitar qualquer problema, vamos remover as receitas que criamos na mão:
```make
main.o: main.c hello_world.o hello_world.h
hello_world.o: hello_world.c hello_world.h
```
Por fim, vamos gerar esse arquivos na hora da compilação, pra isso vamos usar um recurso do compilador, no momento fazemos algo assim:
```make
cc -c codigo.c -o objeto.o
```
O `-o` aí tá indicando um arquivo de saída que é um objeto com o código compilado. Para gerar um arquivo de dependencia usamos a flag `-MMD`, o que ficaria:
```make
cc -c codigo.c -o objeto.o -MMD
```
Nota: `-MMD` significa que um arquivo de dependencia deve ser gerado no processo com uma receita (da mesma forma que fizemos antes na mão), o nome desse arquivo é baseado no nome do arquivo que é indicado em `-o`.
Por isso $(DEPENDENCIES) usa o `%.o` e não o `%.c` para gerar o nome dos arquivos `%.d`, já que os arquivos de objetos podem ficar em locais diferentes de arquivos de origem, mas isso é assunto pra outra hora. Spoiler: Caso queira especificar o nome do arquivo, use `-MF <nome do arquivo>` logo após `-MMD`.

Outra nota: Você pode usar `-MD` caso queira que arquivos de sistema também serão levados em consideração, mas aqui vamos focar somente nos seus arquivos de projeto.

Para mais informações, consulte o manual usando o comando `man cc` ou (recomendo) no site [man7.org](https://man7.org/linux/man-pages/man1/gcc.1.html).

Enfim, fica assim:
```make
%.o: %.c
	$(CC) -c $< -o $@ -MMD
```

<h2></h2> <!-- Line divisor -->

Nesse ponto, seria muito bom fazer algo que está incompleto, a receita `clean`. Ela precisa remover os arquivos intermediários, no caso desse projeto os objetos e dependências, isso fica simples agora:

Nota sobre outra nota: Abaixo tem um nota importante, acho bom ler ela antes de testar :eyes:

```make
clean:
	$(RM) $(DEPENDENCIES) $(OBJECTS)
```
Ao executar `make clean`, a saída esperada será:
```bash
rm -f main.d hello_world.d main.o hello_world.o
```
ou
```make
clean:
	$(RM) $(DEPENDENCIES)
	$(RM) $(OBJECTS)
```
Ao executar `make clean`, a saída esperada será:
```bash
rm -f main.d hello_world.d
rm -f main.o hello_world.o
```

Nota: Cuidado aqui, pois estaremos removendo arquivos, tenha certeza de que está removendo os arquivos certos, um teste simples pode ajudar:
```make
test:
	@echo DEPENDENCIES = $(DEPENDENCIES)
	@echo OBJECTS = $(OBJECTS)
```
Você também pode usar a flag -n, ela apenas mostra os comandos que serão executados sem de fato executar eles
Executando `make clean -n`, a saída esperada será:
```bash
rm -f main.d hello_world.d
rm -f main.o hello_world.o
```
ou
```bash
rm -f main.d hello_world.d
rm -f main.o hello_world.o
```

<h2></h2> <!-- Line divisor -->

Com isso temos um Makefile que sabe o que e quando atualizar em um projeto simples.

Você pode:

Alterar o nome de saída alterando $(NAME).

Mudar configurações como o compilador em $(CC) ou alterar $(RM) para funcionar em pastas com a flag -r.

Adicionar saídas mais descritivas com echo e ocultar comandos com `@`.

Muitas coisas podem ser feitas usando esse Makefile de base, futuramente teremos adendos aqui para deixar o Makefile mais robusto ou adaptar para outras situações.

### Quer apoiar o conteúdo?
Caso queira fazer isso via pix, basta escanear o qrcode

<img src="assets/qrcode-pix.png" alt="QRCode Pix" width="300"/>
