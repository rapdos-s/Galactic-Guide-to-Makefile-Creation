# Primeira compilação

Bom, vamos por a mão na massa, primeiro navegue até o diretório do seu projeto usando o terminal.

Você pode usar o `pwd` para saber onde está e `cd <nome do diretório>` para ir para outro diretório.

Crie um arquivo chamado `Makefile`.

<h2></h2> <!-- Line divisor -->

Para manter as coisas simples, apenas copie o código abaixo para dentro do arquivo Makefile:
```make
test:
	echo "Hello, Makefile!"
```
Nota: Repare que antes de `echo` temos um `tab` e não espaços.

No terminal, estando no diretório do seu Makefile, execute o comando:
```bash
make
```

A saída esperada é:
```bash
echo "Hello, Makefile!"
Hello, Makefile!
```

Se esta foi a saída, significa que você já tem o suficiente para executar comandos dentro do Makefile.

<h2></h2> <!-- Line divisor -->

Agora podemos entender melhor o que está acontecendo.

Basicamente criamos uma forma de chamar o comando echo para imprimir "Hello, Makefile!", explicando cada elemento, temos:

`test` o nome da regra, receita ou alvo, nesse guia chamaremos de receita

`:` separa a receita das dependências (Falaremos disso depois)

`echo "Hello, Makefile!"` é um comando simples para mostrar "Hello, Makefile!" no terminal.

Nomeando tudo ficaria:
```make
receita:
	comando
```

<h2></h2> <!-- Line divisor -->

Podemos colocar mais de um comando em uma receita:
```make
test:
	echo "Este é o primeiro comando"
	echo "Este é o segundo comando"
```

Ao executar o comando `make` no terminal teremos a saída:
```bash
echo "Este é o primeiro comando"
Este é o primeiro comando
echo "Este é o segundo comando" 
Este é o segundo comando
```

<h2></h2> <!-- Line divisor -->

Vemos que os dois comandos foram executados, mas está um pouco poluído, podemos usar `@` para que o make não mostre o comando que está sendo executado, mas devemos ter cuidado, pois isso pode ocultar informações importantes mais a frente.
```make
test:
	@echo "Este é o primeiro comando"
	@echo "Este é o segundo comando"
```

Ao executar o comando `make` no terminal teremos a saída:
```bash
Este é o primeiro comando
Este é o segundo comando
```

<h2></h2> <!-- Line divisor -->

Agora que temos uma receita com mais de um comando, podemos criar mais de uma receita:

```make
um:
	@echo "PRIMEIRO comando da receita UM"
	@echo "SEGUNDO comando da receita UM"

dois:
	@echo "PRIMEIRO comando da receita DOIS"
	@echo "SEGUNDO comando da receita DOIS"
```

A saída esperada é:
```bash
PRIMEIRO comando da receita UM
SEGUNDO comando da receita UM
```

Podemos ver que executamos os comandos da primeira receita, mas a segunda não foi executada.
Isso acontece porque o make executa por padrão a primeira receita.

Para escolher quais receitas devem ser executadas, devemos expecificar no comando make dessa forma:
```bash
make <receita_um> <receita_dois> ... <receita_n>
```

Para ver isso na prática podemos fazer alguns testes:

Executando a receita padrão com `make`, a saída esperada será:
```bash
PRIMEIRO comando da receita UM
SEGUNDO comando da receita UM
```

Executando a primeira receita com `make um`, a saída esperada será:
```bash
PRIMEIRO comando da receita UM
SEGUNDO comando da receita UM
```

Executando a segunda receita com `make dois`, a saída esperada será:
```bash
PRIMEIRO comando da receita DOIS
SEGUNDO comando da receita DOIS
```

Executando mais de uma receita com `make um dois`, a saída esperada será:
```bash
PRIMEIRO comando da receita UM  
SEGUNDO comando da receita UM   
PRIMEIRO comando da receita DOIS
SEGUNDO comando da receita DOIS
```

<h2></h2> <!-- Line divisor -->

Com isso já podemos preparar comandos para compilar e executar códigos, para facilitar vamos criar um "Hello, World!" em C:

Crie um arquivo `main.c` no mesmo diretório que o Makefile:
```c
#include <unistd.h>

int main(void){
    write (1, "Hello, World!\n", 14);
}

```

Não vou entrar em muitos detalhes de como o código funciona, de forma direta ele vai escrever `Hello, World!` na saída padrão do terminal.

Um comando simples para compilar esse arquivo seria:
```bash
gcc main.c
```
Isso pode varia dependendo do compilador que está usando, tenha isso em mente.

Para fazer o make executar esse comando, podemos criar uma receita e chamar ela substituindo o conteúdo do Makefile por:
```make
compilar:
	gcc main.c
```

Executando `make` ou `make compilar`, a saída esperada será:
```bash
gcc main.c
```

Também é esperado que um arquivo apareça no mesmo diretório, normalmente chamado `a.out`.

Para executar o código usamos `./a.out ` no terminal, a saída esperada será:
```bash
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

Nota: Apagar arquivos por engano é bem comum, tome o cuidado de fazer backup ou manter tudo num repositório git. Uma boa forma de verificar se tudo está certo é usar o comando `make -n <receita>`, isso fará o Makefile mostrar o que será executado, mas não vai executar nada, dessa forma é possível verificar se está lidando com os arquivos corretos.

As saídas esperadas:

`make` ou `make compilar`
```bash
cc main.c
```

`make executar`
```bash
./a.out
Hello, World!
```

`make apagar`
```bash
rm a.out
```

Com isso já conseguimos o básico, quase tudo que fazemos num terminal é possível fazer dentro do Makefile, mas existem duas coisas muito importantes num Makefile que farão toda diferença: Dependências e Variáveis.

Nos vemos na próxima parte o/

#### [Básico 02 - Dependência nas receitas e Variáveis](https://github.com/rapdos-s/Galactic-Guide-to-Makefile-Creation/tree/main/basic%2002%20-%20Recipe%20Dependencies%20and%20Variables)

### Quer apoiar o conteúdo?
Caso queira fazer isso via pix, basta escanear o qrcode

<img src="../assets/qrcode-pix.png" alt="QRCode Pix" width="300"/>
