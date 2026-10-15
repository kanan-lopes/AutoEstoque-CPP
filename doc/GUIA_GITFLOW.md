# Publicação e evidências do Gitflow

> Este guia não substitui as ações reais exigidas pelo professor. Para comprovar TDD, registre a sequência: teste falhando (red), implementação mínima (green), melhoria (refactor).

## 1. Criar repositório público no GitHub
Nome sugerido: `autoestoque-cpp` (visibilidade: Public). Não adicionar outro README remoto se o pacote local já tiver um.

## 2. Primeiro push
No terminal, dentro da pasta:
```bash
git init -b master
git add .
git status
git commit -m "chore: estrutura inicial do AutoEstoque"
git remote add origin https://github.com/SEU_USUARIO/autoestoque-cpp.git
git push -u origin master
git switch -c staging
git push -u origin staging
git switch -c develop
git push -u origin develop
```
**Atenção:** o pacote contém implementações prontas para demonstração e estudo. Para comprovar uma evolução TDD real, não basta publicar todos esses arquivos em um único commit; organize e registre testes e implementações em commits separados antes do trabalho avaliado, sem falsificar histórico.

## 3. Board GitHub Projects
Crie um Project vinculado ao repositório e configure quatro opções de Status com os nomes exatos:
- `sprint backlog`
- `doing`
- `waiting acceptance`
- `done`

Se o GitHub criar opções padrão diferentes, renomeie-as; prefira um quadro (board) com quatro colunas, não quatro projetos isolados.

## 4. Duas issues sugeridas
**Issue 1 — Detectar ponto de pedido**
Como gerente, quero identificar quando o estoque atinge o ponto de pedido, para tomar decisões de reposição.
Critérios: abaixo, exatamente igual, acima do ponto. Código em `src/bib.hpp`/`src/bib.cpp`; testes em `test/main.cpp`.

**Issue 2 — Calcular quantidade sugerida de reposição**
Como gerente, quero calcular a quantidade necessária até o estoque máximo, para dimensionar uma reposição.
Critérios: estoque abaixo, igual e acima do máximo.

## 5. Procedimento por issue
1. Criar issue e colocá-la em `sprint backlog`.
2. Mover para `doing` ao iniciar.
3. A partir de `develop`, criar branch como `feat/detectar-ponto-pedido` ou `feat/calcular-reposicao`.
4. Escrever e executar os testes primeiro, registrando a falha (red).
5. Implementar em `src/bib.hpp` e `src/bib.cpp` e executar `make test` até passar (green).
6. Refatorar se necessário; realizar commits de teste e de código com mensagens claras.
7. Integrar a branch da funcionalidade em `develop` e enviar para o remoto; mover issue para `waiting acceptance`.
8. **Parar aqui até a avaliação do P.O.** Após aprovação, mover para `done` e incorporar a funcionalidade a `staging`.

## 6. Evidências recomendadas
- `git status` sem arquivo do projeto untracked;
- `git branch -a`;
- `git log --oneline --graph --all`;
- execução de `make` e `make test`;
- quadro com as issues em seus respectivos estados;
- execução presencial dos testes no aceite.

## 7. Qt
A interface Qt do AutoEstoque será acrescentada em uma sprint negociada. Este Makefile atende à base C++ pura, mas **não compila automaticamente arquivos que exigem Qt/moc**; adaptar o build ao introduzir a interface.
