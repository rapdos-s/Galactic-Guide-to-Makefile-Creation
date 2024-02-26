### O que é Makefile?
De forma direta? É uma forma de definir "comandos" para "fazer" coisas no projeto. Em geral, esse "fazer" abrenge compilar e limpar o projeto todo ou partes dele.
Ou seja, você pode usar coisas como
```
make re
```
no lugar de algo como
```
rm -f a.out && cc main.c hello_world.c -o a.out
```
Veja que os comandos `make` normalmente tem o mesmo comportamente em todos os Makefiles, então fica mais fácil entender e compilar outros projetos.

### O que é esse guia?
De início, vai ser um local onde vou guardar referências e anotações pessoais sobre o assunto, mas a ideia é que fique claro para quem estiver lendo.
A melhor forma que encontrei foi um guia que vai crescendo conforme estudo e reviso, logo acredito que vai conter bastante informações no futuro.

### O que preciso?

**Um editor de texto**: Qualquer um de sua preferência;

**Terminal**: Estou no ambiente Linux Ubuntu (Através do wsl no Windows 10) usando o bash, nada realmente requintado, se estiver usando alguma distribuição Linux ou o wsl, tudo deve funcionar bem;

**make**: É o comando para executar os trecos dentro do Makefile. Pra saber se está instalado use o comando `make --version`;

**Compilador**: Responsável por compilar o código, vou usar o gcc, mas qualquer equivalente pode ser usado. Pra saber se está instalado use o comando `gcc --version`.

### Makefile 101
Nessa sessão temos o básico, terminando as 4 partes teremos um Makefile funcional para um projeto básico em C.
#### [Básico 01 - Primeira Compilação](https://github.com/rapdos-s/Galactic-Guide-to-Makefile-Creation/tree/main/basic%2001%20-%20First%20compilation)
#### [Básico 02 - Dependência nas receitas e Variáveis](https://github.com/rapdos-s/Galactic-Guide-to-Makefile-Creation/tree/main/basic%2002%20-%20Recipe%20Dependencies%20and%20Variables)
#### [Básico 03 - Escrevendo um Makefile Básico](https://github.com/rapdos-s/Galactic-Guide-to-Makefile-Creation/tree/main/basic%2003%20-%20Writing%20a%20basic%20Makefile)
#### [Básico 04 - Finalizando e removendo relink](https://github.com/rapdos-s/Galactic-Guide-to-Makefile-Creation/tree/main/basic%2004%20-%20Removing%20relink%20finishing)
