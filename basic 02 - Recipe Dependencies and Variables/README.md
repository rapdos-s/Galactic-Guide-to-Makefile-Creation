#### Parte anterior : [Básico 01 - Primeira Compilação](https://github.com/rapdos-s/Galactic-Guide-to-Makefile-Creation/tree/main/basic%2001%20-%20First%20compilation)


Partindo do ponto anterior, temos três receitas: `compilar`, `apagar` e `executar`.

É possível reparar nessas receitas que algumas dependem de outras para serem executadas, vamos passar por elas corrigindo isso.

<h2></h2> <!-- Line divisor -->

#### Dependências

Para executar o programa, antes temos que compila-lo.
Podemos ver melhor isso executando `make apagar` seguido de `make executar` ou simplesmentes `make apagar executar`, a saída esperada é:

Para `make apagar`
```bash
rm a.out
rm: cannot remove 'a.out': No such file or directory
make: *** [Makefile:5: apagar] Error 1
```
Para `make executar`
```bash
./a.out
make: ./a.out: No such file or directory
make: *** [Makefile:8: executar] Error 127
```

Para `make apagar executar`
```bash
rm a.out
rm: cannot remove 'a.out': No such file or directory
make: *** [Makefile:5: apagar] Error 1
```

Isso acontece porque `rm` não encontra o arquivo para apagar e a execução **depende** da compilação e precisamos indicar isso de forma explícita no Makefile

Para corrigir o `rm`, adicionamos o `-f`, com isso ele vai apagar sem te pedir permissões e nao vai subir erros caso algum arquivo não exista.

```bash
apagar:
	rm -f a.out
```

<h2></h2> <!-- Line divisor -->

Quanto a dependência em si, vamos lembrar que temos essa estrutura nas receitas:
```make
receita: dependencia
	comando
```

Então podemos indicar na receita `executar` que precisamos antes executar a receita `compilar`
```make
compilar:
	cc main.c

apagar:
	rm -f a.out

executar: compilar
	./a.out
```

Agora a compilação **sempre** será executada antes da execução, não é o ideal, mas é um bom passo, a saída esperada do `make executar` é:
```bash
cc main.c
./a.out
Hello, World!
```

Por que isso não é realmente ideal?
Bom, compilar novamente nem sempre é necessário, logo o ideal é fazer o Makefile só executar comandos como esse quando realmente necessário, veremos isso nos próximos guias quando falarmos de relink.

De forma geral, pensar que algo depende de algo é uma boa forma de definir as dependências
Por exemplo, para compilar é necessário que o arquivo main.c exista, caso ele não exista esse erro ocorreria ao executar `make compilar`:

```bash
cc main.c
cc1: fatal error: main.c: No such file or directory
compilation terminated.
make: *** [Makefile:2: compilar] Error 1
```

Pensando logicamente corrigimos dessa forma:
```make
compilar: main.c
	cc main.c
```

Mas nesse caso main.c não é gerado pois não existem uma receita que cria esse arquivo, ele é criado e escrito por ti, então o erro mudaria, mas o problema não seria de fato resolvido:
```bash
make compilar
make: *** No rule to make target 'main.c', needed by 'compilar'.  Stop.
```

:eyes: Pelo menos não é um erro fatal dessa vez 

<h2></h2> <!-- Line divisor -->

#### Variáveis

Se repararmos bem, veremos que main.c e a.out são referênciados duas vezes cada um. Nesse caso é simples alterar para outros nomes caso necessário, mas em projetos maiores isso poderia ser um problema. Outra questão frequente é ter uma forma de indicar quais comandos, flags, parâmetros ou configurações usaremos nesse processo. Para isso podemos fazer uso de variáveis.

Em Makefile não temos um tipo de variáveis, você pode interpretar elas como o tipo string ou o um array de strings separadas por espaços em alguns casos.

Para declarar variáveis seguimos esse padrão:
```make
VARIAVEL = valor
ARRAY = valor_um valor_dois valor_tres
```
ou
```make
VARIAVEL := valor
ARRAY := valor_um valor_dois valor_tres
```

Não é necessário usar caixa alta (maiúsculas), mas é uma convênção e facilita a vida.

Para usar esse valor usamos `$` e o nome da variável entre parênteses ou chaves, nos exemplos acima ficaria `$(VARIAVEL)` ou `${VARIAVEL}` e `$(ARRAY)` ou `${ARRAY}`. Eu sinceramente nunca li nada sobre a diferença entre o uso de parenteses e chaves, também não vi nenhuma diferença nos meus testes.

A diferença entre `=` e `:=` é que `:=` considera que o valor anterior da variável pode ser usado na atribuição do novo valor. Isso significa que a variável pode receber o valor dela mesma concatenado com outro valor ou usar o valor anterior de base para o novo valor. Ex.:

```make
FOO = um
BAR = dois
BAZ = $(FOO) $(BAR)
BAR := $(BAR) tres

test:
	@echo "FOO = $(FOO)"
	@echo "BAR = $(BAR)"
	@echo "BAZ = $(BAZ)"
```

`FOO` possui o valor de `um`
`BAR` primeiro recebeu `dois` e depois recebeu o valor anterior dele mesmo e `tres` ficando `dois tres`
`BAZ` recebeu o valor de `FOO`, ou seja, `um` e o valor de `BAR`, ou seja, `dois tres`.

Note que `BAR` precisou de `:=` porque faz referência a ele mesmo (Recursão :eyes:).

Note também que `BAZ` não recebeu o valor anterior de `BAR`. Isso acontece porque o valor só vai ser expandido na hora que for usado. Isso significa que ele não recebe uma cópia do valor, mas sim que deve consultar esse valor na hora que precisar pegando o valor mais atual

Agora podemos usar variáveis para guardar valores que serão usados em receitas ou nas próprias variáveis.

Para indicar o arquivo de origem explícitamente:
```make
SOURCE = main.c
```
Para indicar o arquivo de origem usando uma função que vai ler todos os arquivos `.c` na pasta:
```make
SOURCE = $(wildcard *.c)
```

Para indicar o arquivo de saída
```make
NAME = a.out
```

O resultado é algo assim:
```make
SOURCE = $(wildcard *.c)
NAME = a.out

compilar: $(SOURCE)
	cc $(SOURCE)

apagar:
	rm $(NAME)

executar: compilar
	./$(NAME)
```

Note que ao executar o `make apagar executar`, as variáveis aparecerão com o próprio valor já expandido:
```bash
rm -f a.out
cc main.c  
./a.out
Hello, World!
```

Com isso já podemos criar Makefiles mais robustos.

A seguir faremos um Makefile suficiente para lidar com um projeto simples.

Nos vemos na próxima parte o/

#### [Básico 03 - Escrevendo um Makefile Básico](https://github.com/rapdos-s/Galactic-Guide-to-Makefile-Creation/tree/main/basic%2003%20-%20Writing%20a%20basic%20Makefile)
