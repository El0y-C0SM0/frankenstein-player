Para quem deseja colaborar com o projeto, este arquivo reúne informações relevantes sobre a organização e configuração do projeto
## Para contribuir
Confira no quadro de projetos o que há de disponível para contribuir e separe uma branch para isso. Ao fim do trabalho solicite um Pull Request (PR). Se aceito um PR, a branch deve ser excluída se não for mais necessária (comportamento padrão), **se desejar que a branch não seja excluída, notifique isso no PR**
## Branches
Para cada issue em progresso, recomenda-se o uso de branches da main ou de um outro issue superior. Atribua a essas branches nomes descritivos, de preferência com o seguinte padrão se for aplicável:
```
<issue type>/<short description>
```
Como os exemplos:
- `chore/update-SQLiteCPP-in-CMake`;
- `task/implementação-da-classe-user`;
- `bugfix/fix-audio-sync`;
Abaixo consta alguns tipos de issue aplicáveis

| **Tipo** | **Descrição**            |
| -------- | ------------------------ |
| feature  | Nova funcionalidade      |
| bugfix   | Correção de bug          |
| task     | Realização de uma tarefa |
| hotfix   | Correção urgente         |
| release  | Preparação de release    |
| docs     | Documentação             |
| refactor | Refatoração              |
| test     | Testes                   |
| chore    | Tarefas de manutenção    |
## Commits
Para facilitar a revisão de Pull Requests, saber identificar bem os commits é uma prática mais que bem vinda. Logo, sempre que aplicável, recomenda-se o uso do padrão:
```
<type>(optional scope): <short description>
```
Por exemplo:
- `feat: add scroble API`;
- `docs: update do tutorial de instalção`;
- `fix (interface): correção da criação de playlists`.
Em commits que entregam muitas modificações, também é recomendável uma descrição melhor no corpo do commit.
Abaixo segue a tabela de tipos de commits que podem ser usados.

| Tipo     | Código                 | Descrição                                       |
| -------- | ---------------------- | ----------------------------------------------- |
| feat     | `:sparkles: feat`      | Nova funcionalidade                             |
| task     | `:memo: task`          | Tarefa completa                                 |
| fix      | `:bug: fix`            | Correção de bug                                 |
| docs     | `:books: docs`         | Alterações na documentação                      |
| style    | `:lipstick: style`     | Formatação do código                            |
| refactor | `:recycle: refactor`   | Refatoração de código                           |
| test     | `:test_tube: test`     | Adição ou correção de testes                    |
| chore    | `chore`                | Atualização de tarefas de build e configurações |
| perf     | `:zap: perf`           | Melhorias de performance                        |
| build    | `:wrench: build`       | Mudanças no sistema de build ou dependências    |
| security | `:lock: security`      | Atualizações de segurana                        |
| remove   | `:wastebasket: remove` | Remoção de arquivos                             |
|          | `:boom: revert`        | Revertendo mudanças                             |
