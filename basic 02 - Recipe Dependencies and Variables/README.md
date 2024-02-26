Partindo do ponto anterior, temos trÊs receitas: compilar, apagar e executar

Dependências:

Para executar o programa, antes temos que compila-lo.
Podemos ver melhor isso executando make apagar seguido de make executar ou simplesmentes make apagar executar

A saída esperada é:
```sh
make apagar
rm a.out
```
```sh
make executar
./a.out
make: ./a.out: No such file or directory
make: *** [Makefile:8: executar] Error 127
```

ou
```sh
make apagar executar
rm a.out
rm: cannot remove 'a.out': No such file or directory
make: *** [Makefile:5: apagar] Error 1
```

Isso acontece porque a execução **depende** da compilação e precisamos indicar isso de forma explícita no Makefile

Lembrando que temos:

```make
receita: dependencia
	comando
```

Entõa podemos indicar na receita `executar` que precisamos antes executar a receita `compilar`

```make
compilar:
	cc main.c

apagar:
	rm a.out

executar: compilar
	./a.out
```

Agora a compilação SEMPRE será executada antes da execução, não é o ideal, mas é um bom passo.
```sh
make executar
cc main.c
./a.out
Hello, World!
```

Por que isso não é realmente ideal?
Bom, compilar novamente nem sempre é necessário, lego o ideal é fazer o Makefile só executar comandos como esse quando realmente necessário, veremos isso nos próximos guias.

De forma geral, pensar que algo depende de algo é uma boa forma de definir as dependências
Por exeplo, para compilar é necessário que o arquivo main.c exista, caso ele não exista esse erro ocorreria:

```
make compilar
cc main.c
cc1: fatal error: main.c: No such file or directory
compilation terminated.
make: *** [Makefile:2: compilar] Error 1
```

Pensando logicamente corrigimos dessa forma:
```
compilar: main.c
```

Mas nesse caso main.c não é gerado pois não existem uma receita que cria esse arquivo, ele é criado e escrito por ti, então o erro mudaria, mas o probelma não seria de fato resolvido:

```
make compilar
make: *** No rule to make target 'main.c', needed by 'compilar'.  Stop.
```

:eyes: Pelo menos não é um erro fatal dessa vez

Variáveis

Se repararmos bem, veremos que main.c e a.out são referênciados duas vezes cada um. Nesse caso é simples alterar para outros nomes caso necessário, mas em projetos maiores isso poderia ser um problema.

Outra questão frequênte é ter uma forma de indicar quais comandos, flags, parâmetros ou configurações usaremos nesse processo.

Para isso podemos fazer uso de variáveis.

Em Makefile não temos um tipo de variáveis, você pode interpretar elas como o tipo string ou o um array de strings separadas por espaços em alguns casos.

Para declarar variáveis seguimos esse padrão:
```
VARIAVEL = valor
ARRAY = valor_um valor_dois valor_tres
```
ou
```
VARIAVEL := valor
ARRAY := valor_um valor_dois valor_tres
```

Não é necessário usar caixa alta (maiúsculas), mas é uma convênção e facilita a vida.

Para usar esse valor usamos `$` e o nome da variável entre parênteses ou chaves

A diferença entre `=` e `:=` é que `:=` considera que o valor anterior da variável pode ser usado na atribuição do novo valor. Isso significa que a variável pode receber o valor dela mesma mais algo ou usar o valor anterior de base para o novo valor. Ex.:

```
FOO = um
BAR = dois
BAZ = $(FOO) $(BAR)
BAR := $(BAR) tres

test:
	@echo "FOO = $(FOO)"
	@echo "BAR = $(BAR)"
	@echo "BAZ = $(BAZ)"
```

FOO possui o valor de "um"
BAR primeiro recebeu "dois" e depois recebeu o valor anterior dele mesmo e "tres" ficando "dois tres"
BAZ recebeu o valor de FOO ("um") e o valor de BAR ("dois tres").

Note que BAR precisou de ":=" porque faz referência a ele mesmo.
Note também que BAZ não recebeu o valor anterior de BAR. Isso acontece porque o valor só vai ser expandido na hora que for usado. Isso significa que ele não recebe uma cópia do valor, mas sim que deve consultar esse valor na hora que precisar pegando o valor mais atual

Agora podemos usar variáveis para guardar valores que serão usados em receitas ou nas próprias variáveis.

Para indicar o arquivo de origem
```
SOURCE = main.c
```

Para indicar o arquivo de saída
```
NAME = a.out
```

O resultado é algo assim:

```
SOURCE = main.c
NAME = a.out

compilar: $(SOURCE)
	cc $(SOURCE)

apagar:
	rm $(NAME)

executar: compilar
	./$(NAME)
```

Note que ao executar o make, as variáveis aparecerão com o próprio valor já expandido:
```
make compilar
cc main.c
```

Com isso já podemos criar Makefiles mais completos.
Na próxima parte faremos um Makefile completo suficiente para um projeto simples.