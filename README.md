# Rede Neural para Classificação de Feijões Secos (Dry Bean Dataset)

## 📊 Dataset
**Fonte:** [UCI Machine Learning Repository - Dry Bean Dataset](https://archive.ics.uci.edu/dataset/602/dry+bean+dataset)

### Informações Gerais
- **Número de instâncias:** 13.611
- **Número de atributos originais:** 17 (16 características + 1 classe)
- **Classes:** 7 tipos de feijões secos
  - SEKER
  - BARBUNYA
  - BOMBAY
  - CALI
  - DERMASON
  - HOROZ
  - SIRA
- **Tipo de tarefa:** Classificação multiclasse
- **Valores ausentes:** Não

### Descrição
Imagens de 13.611 grãos de 7 diferentes variedades registradas de feijões secos foram capturadas com uma câmera de alta resolução. Um total de 16 características foram extraídas dos grãos: 12 dimensões e 4 formas geométricas.

### Atributos do Dataset Original
1. **Area (A):** Área da região do feijão (número de pixels)
2. **Perimeter (P):** Perímetro do feijão (comprimento da borda)
3. **Major Axis Length (L):** Distância entre as extremidades da linha mais longa
4. **Minor Axis Length (l):** Linha mais longa perpendicular ao eixo principal
5. **Aspect Ratio (K):** Relação entre L e l
6. **Eccentricity (Ec):** Excentricidade da elipse equivalente
7. **Convex Area (C):** Pixels no menor polígono convexo
8. **Equivalent Diameter (Ed):** Diâmetro de um círculo com mesma área
9. **Extent (Ex):** Razão dos pixels na caixa delimitadora
10. **Solidity (S):** Convexidade (razão pixels no envoltório convexo)
11. **Roundness (R):** Calculado como (4πA)/(P²)
12. **Compactness (CO):** Mede a circularidade: Ed/L
13. **ShapeFactor1 (SF1)**
14. **ShapeFactor2 (SF2)**
15. **ShapeFactor3 (SF3)**
16. **ShapeFactor4 (SF4)**

## 🧠 Arquitetura da Rede Neural

### Atributos Selecionados (6 features)
Para este projeto, foram selecionadas 6 características do dataset original:

1. **ConvexArea (C)** - Alta importância, baixa correlação com outras
2. **EquivDiameter (Ed)** - Característica dimensional importante
3. **Solidity (S)** - Feature independente de forma
4. **ShapeFactor1 (SF1)** - Fator de forma relevante
5. **ShapeFactor2 (SF2)** - Característica de forma complementar
6. **ShapeFactor4 (SF4)** - Característica geométrica adicional

### Estrutura da Rede (implementada em `beans_code.c`)

```
┌─────────────────────────────────────────────────────────────┐
│                   CAMADA DE ENTRADA                         │
│                 6 neurônios + 1 bias = 7                    │
│  [ConvexArea, EquivDiameter, Solidity, SF1, SF2, SF4, bias] │
└──────────────────────┬──────────────────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────────────────┐
│                   CAMADA OCULTA                             │
│                 5 neurônios + 1 bias = 6                    │
│              Função de Ativação: Sigmoide                   │
└──────────────────────┬──────────────────────────────────────┘
                       │
                       ▼
┌─────────────────────────────────────────────────────────────┐
│                   CAMADA DE SAÍDA                           │
│                      7 neurônios                            │
│              Função de Ativação: Sigmoide                   │
│         [SEKER, BARBUNYA, BOMBAY, CALI,                     │
│              DERMASON, HOROZ, SIRA]                         │
└─────────────────────────────────────────────────────────────┘
```

**Diagrama visual:** `neural-network-image.jpg`

### Parâmetros de Treinamento
- **Taxa de aprendizado (Learning Rate):** 0.01
- **Momentum:** 0.9
- **Algoritmo:** Backpropagation
- **Épocas:** 2.000
- **Iterações por época:** 15.000
- **Divisão dos dados:**
  - Treinamento: 70% (9.528 amostras)
  - Teste: 30% (4.083 amostras)

### Justificativa da Seleção de Features
- ✅ Combina features com **alta importância individual**
- ✅ Inclui features **independentes** para máxima diversidade
- ✅ **Evita redundância** através de baixa correlação entre features
- ✅ Mantém features com **baixo número de outliers**
- ✅ **Redução dimensional** significativa: 62.5% (de 16 para 6 features)

## 📚 Referência
**KOKLU, M. and OZKAN, I.A.** (2020), "Multiclass Classification of Dry Beans Using Computer Vision and Machine Learning Techniques." *Computers and Electronics in Agriculture*, 174, 105507.

DOI: [https://doi.org/10.1016/j.compag.2020.105507](https://doi.org/10.1016/j.compag.2020.105507)