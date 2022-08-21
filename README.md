# Tradutor bpl-assembly

Este foi um trabalho da disciplina de Software Básico, com o intuito de reforçar o conhecimento de assembly.

A linguagem bpl, foi inventada pelo professor da displina. A ideia é que através desta, será extraído um padrão similar ao de C e por fim transformado em assembly.

Portanto, o funcionamento dar-se da seguinte forma. Um arquivo .bpl é recebido e são extraídos padrões destes. Estes padrões são convertidos para assembly e escritos em um arquivo.

Meu intuito com este repositório, é que ele possa facilitar o aprendizado de alguém que esteja tentando aprender assembly, visto que eu mesma não achei muito fácil o entendimento de tal assunto.

Como executar:

1. Será necessário gerar o executável do tradutor. Como eu criei um .h é necessário passá-lo como parâmetro na execução.

        gcc -o tradutor tradutor.c biblioteca/imp_funcoes.c
2. Para passar o arquivo que será traduzido e o arquivo onde será escrito a tradução, digite:

        ./tradutor <arquivos_teste/prog.bpl> arquivos_teste/prog.S
3. Gerar o executável do arquivo main, ou seja, do arquivo que faz uso das funções traduzidas. Lembre de entrar na pasta *arquivos_teste*

        gcc -o main main.c prog.S
4. Agora para verificar se tudo funcionou, basta executar

        ./main

*Observação: Não foi implementado a chamada de função, mas caso queira, um conselho seria trocar o uso de registradores amarelos por verdes. Um exemplo para se inspirar segue abaixo.*

        function f6 pa1
        def
        var vi1
        var vi2
        vet va3 size ci30
        enddef
        vi1 = ci1
        vi2 = call f5 vi1 va3
        vi2 = call f5 ci5 pa1
        return vi2
        end
