# 📚 Sistema de Gerenciamento de Livros em C

![C](https://img.shields.io/badge/Linguagem-C-blue)
![Status](https://img.shields.io/badge/Status-Funcional-success)
![Armazenamento](https://img.shields.io/badge/Armazenamento-Arquivo%20Binário-orange)

Sistema desenvolvido em **C** para gerenciamento de um acervo de livros de uma biblioteca.  
Permite **cadastrar**, **listar**, **pesquisar**, **ordenar** e **salvar** livros com persistência em arquivo binário.

---

## ✨ Funcionalidades

✅ Cadastro de livros  
✅ Listagem completa do acervo  
✅ Pesquisa de livros por código  
✅ Ordenação por ano de publicação  
✅ Salvamento automático em arquivo  
✅ Carregamento automático ao iniciar

---

## 🗂️ Estrutura do Livro

Cada livro é representado por uma `struct` contendo:

- 📌 Código
- 📖 Título
- ✍️ Autor
- 🧠 Área / Categoria
- 📅 Ano de publicação
- 🏢 Editora

```c
struct Livro {
    int codigo;
    char titulo[50];
    char autor[30];
    char area[30];
    int ano;
    char editora[30];
};
```
