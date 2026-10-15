# AutoEstoque — C++ / Qt (base de desenvolvimento)

Projeto acadêmico de Engenharia de Software (UFOP), baseado no DER AutoEstoque v1.0.

## Objetivo
Construir progressivamente um sistema de controle de estoque para uma rede de lojas de informática. O sistema completo prevê cadastro, configuração do ponto de pedido, monitoramento de estoque, consulta entre filiais e apoio à reposição. **Este repositório inicial contém apenas a base de linha de comando e duas funções de domínio**, não a interface Qt nem o sistema completo.

## Estrutura
```
AutoEstoque-CPP/
├── src/       # código C++ e arquivo principal
├── include/   # cabeçalhos públicos (reservado)
├── bin/       # objetos e executáveis ignorados pelo Git
├── test/      # testes de regressão em C++
├── doc/       # DER e demais documentos (inserir somente se autorizados)
├── Makefile
├── .gitignore
└── README.md
```

## Requisitos de ambiente
- Compilador C++17 (`g++`) e `make` disponíveis no terminal.
- No Windows, usar MSYS2/MinGW compatível com o compilador escolhido para Qt. O Makefile é propositalmente independente do Qt por enquanto; a futura aplicação gráfica exigirá ajustar o processo de build (por exemplo, CMake/Qt Creator).

## Compilação e execução
```bash
make
./bin/autoestoque
make test
make clean
```
No Windows (MinGW): `bin\autoestoque.exe` e `bin\testeRegressivo.exe`.

`make` compila todos os arquivos `.cpp` de `src`, gerando `.o` em `bin`. `make test` compila separadamente os testes (`test/main.cpp`) com o código de produção, sem `src/main.cpp`.

## Funcionalidades de demonstração TDD
- `estoqueAtingiuPontoPedido(quantidadeAtual, pontoPedido)`: sinaliza estoque em ou abaixo do ponto de pedido.
- `calcularQuantidadeReposicao(quantidadeAtual, estoqueMaximo)`: calcula unidades para completar o estoque máximo, nunca negativas.

**São funções de domínio isoladas:** não registram vendas, não monitoram automaticamente um banco de dados e não enviam pedidos. São candidatas a duas issues e futuras integrações à interface Qt.

## Gitflow solicitado na disciplina
- `master`: base principal aprovada;
- `develop`: integração de funcionalidades em desenvolvimento;
- `staging`: funcionalidades após aceite do P.O.;
- `feat/nome-da-funcionalidade`: nasce de `develop`, recebe implementação e testes, depois é incorporada a `develop`.

Workflow por issue: `sprint backlog` → `doing` → `waiting acceptance` → `done` **somente após o aceite do professor**. Não mover para `done` nem incorporar a `staging` antes da avaliação.

Consulte `doc/GUIA_GITFLOW.md` para o roteiro de publicação, branches e evidências.

## Critérios de aceite das funções iniciais
- Verificar o ponto de pedido exatamente no limite, abaixo e acima.
- Calcular reposição quando o estoque está abaixo, igual ou acima do máximo.
- `make test` deve encerrar sem erros.

## Situação
Base local demonstrável; publicação no GitHub, histórico real TDD, configuração de board e aceite do P.O. pendentes. A simples presença do código e dos testes **não comprova** que o processo TDD e as movimentações de issue foram realizados.
