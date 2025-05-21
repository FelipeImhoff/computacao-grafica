# Remoção de Faces Ocultas com OpenGL

Este projeto é uma aplicação simples em C++ com OpenGL que demonstra três técnicas de **remoção de faces ocultas**:

- **Back-Face Culling**
- **Z-Buffer**
- **Painter's Algorithm**

## 📸 Visão Geral

O programa renderiza quatro cubos posicionados em diferentes profundidades e permite ao usuário alternar entre as técnicas de renderização para observar as diferenças visuais entre elas.

## 🎯 Técnicas Implementadas

### 1. **Back-Face Culling**

Remove as faces "traseiras" de cada objeto (aquelas que não estão voltadas para a câmera).  
Utiliza `glEnable(GL_CULL_FACE)` e `glCullFace(GL_BACK)` para ativar o descarte.

### 2. **Z-Buffer**

Usa um **buffer de profundidade** para garantir que apenas o pixel mais próximo da câmera seja desenhado.  
Utiliza `glEnable(GL_DEPTH_TEST)`.

### 3. **Painter’s Algorithm**

Desenha manualmente os objetos de trás para frente, sobrepondo-os conforme necessário.  
Essa técnica não usa buffer de profundidade e exige ordenação manual das primitivas.

---

## 🕹️ Controles

- **Teclas Numéricas**

  - `1` → Ativa **Back-Face Culling**
  - `2` → Ativa **Z-Buffer**
  - `3` → Ativa **Painter's Algorithm**

- **Setas do Teclado**
  - `←` → Rotaciona a cena para a esquerda
  - `→` → Rotaciona a cena para a direita

---

## 🧱 Dependências

Este projeto usa **OpenGL com GLUT**. Para compilar, você precisa das seguintes bibliotecas instaladas:

- OpenGL (`libGL`)
- GLUT (`freeglut` ou similar)

### Em sistemas baseados em Debian/Ubuntu:

```bash
sudo apt update
sudo apt install freeglut3-dev

```
