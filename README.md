# 💧 Simulador de Hidrômetro Residencial

## 🌟 1. Introdução
Este projeto apresenta um **Simulador de Hidrômetro Residencial**, desenvolvido em **C++**, para reproduzir de forma aproximada o funcionamento de um hidrômetro real. O sistema permite monitorar o consumo de água em uma residência, considerando diferentes cenários típicos do dia a dia.

O simulador é uma ferramenta prática e intuitiva. Com ele, é possível acompanhar e registrar dados em tempo real, entender hábitos de uso e o **consumo de água**.

Principais objetivos do projeto:

- 📊 Simular o consumo residencial de água.  
- 🏠 Avaliar diferentes cenários do dia a dia.  
- 🔄 Gerar dados aleatórios de vazão e presença de ar.  

---

## ⚙️ 2. Características Técnicas
O simulador considera diversos fatores que influenciam a medição do consumo de água, todos configuráveis e gerados de forma aleatória para permitir simulações realistas.

**Principais funcionalidades configuráveis:**

1. 💨 **Simulação de ar no ramal:**  
   Caso o hidrômetro detecte ar, apenas 10% da vazão é contabilizada, sendo possível ajustar esse parâmetro.

2. 💦 **Vazão variável:**  
   A vazão é gerada aleatoriamente dentro de um intervalo definido, que pode ser ajustado conforme a necessidade.

3. ⚖️ **Entrada de água ou ar:**  
   O sistema decide aleatoriamente se o que entra é água ou ar, com maior probabilidade para a água. Esse comportamento também pode ser ajustado.

4. ⏱️ **Atualização de consumo:**  
   O consumo é exibido em intervalos de tempo configuráveis (`t` segundos) no display, mostrando o total até o momento da atualização.

---

## 🛠️ 3. Instalação

1. Clone o repositório:
```bash
git clone https://github.com/seu-usuario/simulador-hidrometro.git
