# Sistema de Gerenciamento de Música

Um sistema completo para gerenciamento local de músicas, álbuns, artistas e reprodução de áudio desenvolvido em C++.

## 📖 Documentação

A documentação completa do projeto está disponível em:  
**[Documentação Doxygen](docs/output/latex/refman.pdf)**

## 🏗️ Estrutura do Projeto

### Entidades Principais

#### 🎵 Song (Música)
- Representa uma música com metadados completos
- Suporte para título, artista, álbum, duração, gênero e ano
- Futura integração com TagLib para extração automática de metadados

#### 🎤 Artist (Artista)
- Gerencia informações de artistas/bandas
- Mantém coleções de músicas e álbuns
- Fornece estatísticas e buscas no catálogo

#### 💿 Album (Álbum)
- Representa coleções de músicas de um artista
- Mantém ordem original das faixas
- Calcula durações totais e fornece navegação sequencial

#### ▶️ Player (Reprodutor)
- Controle de reprodução de áudio
- Playlist management
- Controles de play, pause, next, previous e volume

#### 🏷️ Entity (Interface)
- Interface base para todas as entidades do sistema
- Fornece ID único e operações de comparação

## 📋 Funcionalidades

### Gerenciamento de Conteúdo
- ✅ Cadastro e edição de músicas, álbuns e artistas
- ✅ Busca por nome, gênero e outros metadados
- ✅ Cálculo automático de durações
- ✅ Formatação de informações para exibição

### Reprodução de Áudio
- ✅ Controles básicos de reprodução
- ✅ Gerenciamento de playlists
- ✅ Controle de volume e mute
- ✅ Navegação sequencial em álbuns

### Estatísticas
- ✅ Contagem de músicas e álbuns
- ✅ Duração total de artistas e álbuns
- ✅ Formatação de tempos (HH:MM:SS)

## 🚀 Como Usar

### Pré-requisitos
- Compilador C++11 ou superior
- Doxygen (para documentação)

### Geração da Documentação
```bash
# Gere a documentação
cd docs/
doxygen Doxyfile

# A documentação estará em:
# docs/html/index.html
```

## 👥 Autores

- **Bruno Vieira**
- **Eloy Maciel**
- **João Tavares**
- **Julia Reis**
- **Pedro Barruetavena**

**📚 Documentação Detalhada:** [docs/output/latex/refman.pdf](docs/output/latex/refman.pdf)  
