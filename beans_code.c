#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>

#define N_ENTRADAS 7 // 6 entradas + bias
#define N_OCULTOS 6 // 5 ocultos + bias
#define N_SAIDAS 7 // 7 saídas

int main()
{
    int vezTipo = 0;
    int i = 0;
    int j, l;
    int n, m;
    int cs;
    float er;

    float I0[N_ENTRADAS+1], O0[N_ENTRADAS+1], I1[N_OCULTOS+1], O1[N_OCULTOS+1], I2[N_SAIDAS+1], O2[N_SAIDAS+1];

    float w1[N_ENTRADAS+1][N_OCULTOS+1], w2[N_OCULTOS+1][N_SAIDAS+1];
    float vw1[N_ENTRADAS+1][N_OCULTOS+1], vw2[N_OCULTOS+1][N_SAIDAS+1];
    float d2[N_SAIDAS+1], d1[N_OCULTOS+1];

    float E1[9528], E2[9528], E3[9528], E4[9528], E5[9528], E6[9528];
    float t1[9528], t2[9528], t3[9528], t4[9528], t5[9528], t6[9528], t7[9528];
    
    // Arrays para dados de teste
    float E1_test[4083], E2_test[4083], E3_test[4083], E4_test[4083], E5_test[4083], E6_test[4083];
    float t1_test[4083], t2_test[4083], t3_test[4083], t4_test[4083], t5_test[4083], t6_test[4083], t7_test[4083];

    int indices_tipo[N_SAIDAS+1][4083];
    int count_tipo[N_SAIDAS+1];

    clock_t inicio, fim;
    double tempo_exec;

    inicio = clock();

    // LEITURA DOS DADOS DE TREINAMENTO
    FILE *in_treino;

    if ((in_treino = fopen("beansdata_treino_70.txt", "rt")) == NULL)
    {
        printf("Cannot open training file.\n");
        return 1;
    }

    printf("Iniciando leitura dos dados de treinamento...\n");

    while (!feof(in_treino))
    {
        fscanf(in_treino, "%f%f%f%f%f%f%f%f%f%f%f%f%f", &E1[i], &E2[i], &E3[i], &E4[i], &E5[i], &E6[i], &t1[i], &t2[i], &t3[i], &t4[i], &t5[i], &t6[i], &t7[i]);
        i++;
    }

    int total_samples_treino = i - 1;
    printf("Total de amostras de treinamento: %d\n", total_samples_treino);
    fclose(in_treino);

    FILE *in_teste;
    int i_test = 0;

    if ((in_teste = fopen("beansdata_teste_30.txt", "rt")) == NULL)
    {
        printf("Cannot open test file.\n");
        return 1;
    }

    printf("Iniciando leitura dos dados de teste...\n");

    while (!feof(in_teste))
    {
        fscanf(in_teste, "%f%f%f%f%f%f%f%f%f%f%f%f%f", &E1_test[i_test], &E2_test[i_test], &E3_test[i_test], &E4_test[i_test], &E5_test[i_test], &E6_test[i_test], &t1_test[i_test], &t2_test[i_test], &t3_test[i_test], &t4_test[i_test], &t5_test[i_test], &t6_test[i_test], &t7_test[i_test]);
        i_test++;
    }

    int total_samples_teste = i_test - 1;
    printf("Total de amostras de teste: %d\n", total_samples_teste);
    fclose(in_teste);

    for (int tipo = 1; tipo <= N_SAIDAS; tipo++)
    {
        count_tipo[tipo] = 0;
    }

    for (j = 0; j < total_samples_treino; j++)
    {
        if (t1[j] == 1.0)
        {
            indices_tipo[1][count_tipo[1]] = j;
            count_tipo[1]++;
        }
        else if (t2[j] == 1.0)
        {
            indices_tipo[2][count_tipo[2]] = j;
            count_tipo[2]++;
        }
        else if (t3[j] == 1.0)
        {
            indices_tipo[3][count_tipo[3]] = j;
            count_tipo[3]++;
        }
        else if (t4[j] == 1.0)
        {
            indices_tipo[4][count_tipo[4]] = j;
            count_tipo[4]++;
        }
        else if (t5[j] == 1.0)
        {
            indices_tipo[5][count_tipo[5]] = j;
            count_tipo[5]++;
        }
        else if (t6[j] == 1.0)
        {
            indices_tipo[6][count_tipo[6]] = j;
            count_tipo[6]++;
        }
        else if (t7[j] == 1.0)
        {
            indices_tipo[7][count_tipo[7]] = j;
            count_tipo[7]++;
        }
    }

    for (int tipo = 1; tipo <= N_SAIDAS; tipo++)
    {
        printf("Tipo %d: %d amostras\n", tipo, count_tipo[tipo]);
    }

    // fclose(in);
    // getch();

    for (j = 1; j <= N_OCULTOS-1; j++) // quantidade de saidas de neuronios
    { 
        for (i = 1; i <= N_ENTRADAS; i++) // neuronios na camada
        { 
            w2[j][l]=((rand()%100)/10.0)-5.0;
            vw2[j][l]=0.0;
        }
    }

    for (l = 1; l <= N_SAIDAS; l++) // quantidade de saidas de neuronios
    { 
        for (j = 1; j <= N_OCULTOS; j++) // neuronios na camada
        { 
            w2[j][l]=((rand()%100)/10.0)-5.0;
            vw2[j][l]=0.0;
        }
    }   

    float learning_rate = 0.01;  // Taxa de aprendizado
    float momentum = 0.9;        // Momentum
    
    printf("Iniciando treinamento com LR=%.3f e Momentum=%.2f\n", learning_rate, momentum);

    for (m = 0; m <= 2000; m++)
    {
        er = 0.0;
        for (n = 0; n <= 15000; n++)
        {
            vezTipo = vezTipo + 1;
            if (vezTipo > N_SAIDAS)
                vezTipo = 1;

            // BACK PROPAGATION

            // Selecionar amostra do tipo específico (vezTipo)
            if (count_tipo[vezTipo] > 0)
            {
                int indice_aleatorio = rand() % count_tipo[vezTipo];
                cs = indices_tipo[vezTipo][indice_aleatorio];
            }
            else
            {
                // Fallback para seleção aleatória se não houver amostras do tipo
                cs = rand() % total_samples_treino;
            }

            I0[1] = E1[cs];
            I0[2] = E2[cs];
            I0[3] = E3[cs];
            I0[4] = E4[cs];
            I0[5] = E5[cs];
            I0[6] = E6[cs];
            I0[N_ENTRADAS] = 1.0;

            for (i = 1; i <= N_ENTRADAS; i++)
                O0[i] = I0[i];

            for (j = 1; j <= N_OCULTOS-1; j++)
            { // quantidade de saidas de neuronios (sem bias)
                I1[j] = 0.0;

                for (i = 1; i <= N_ENTRADAS; i++)
                { // neuronios na camada
                    I1[j] += O0[i] * w1[i][j];
                }

                O1[j] = 1.0 / (1.0 + exp(-I1[j]));
            }

            I1[N_OCULTOS] = 1.0; // bias da camada oculta
            O1[N_OCULTOS] = I1[N_OCULTOS];

            for (l = 1; l <= N_SAIDAS; l++)
            { // quantidade de saidas de neuronios
                I2[l] = 0.0;

                for (j = 1; j <= N_OCULTOS; j++)
                { // neuronios na camada
                    I2[l] += O1[j] * w2[j][l];
                }

                O2[l] = 1.0 / (1.0 + exp(-I2[l]));
            }

            // BACK PROPAGATION
            // d2 = gradiente de erro da camada de saida
            d2[1] = (t1[cs] - O2[1]) * O2[1] * (1.0 - O2[1]);
            d2[2] = (t2[cs] - O2[2]) * O2[2] * (1.0 - O2[2]);
            d2[3] = (t3[cs] - O2[3]) * O2[3] * (1.0 - O2[3]);
            d2[4] = (t4[cs] - O2[4]) * O2[4] * (1.0 - O2[4]);
            d2[5] = (t5[cs] - O2[5]) * O2[5] * (1.0 - O2[5]);
            d2[6] = (t6[cs] - O2[6]) * O2[6] * (1.0 - O2[6]);
            d2[7] = (t7[cs] - O2[7]) * O2[7] * (1.0 - O2[7]);

            // d1 = gradiente de erro da camada oculta
            for (j = 1; j <= N_OCULTOS-1; j++)
            {
                float soma = 0.0;
                for (l = 1; l <= N_SAIDAS; l++)
                {
                    soma += d2[l] * w2[j][l];
                }
                d1[j] = O1[j] * (1.0 - O1[j]) * soma;
            }

            for (l = 1; l <= N_SAIDAS; l++)
            { // quantidade de saidas de neuronios
                for (j = 1; j <= N_OCULTOS; j++)
                { // neuronios na camada
                    float delta_w = learning_rate * d2[l] * O1[j] + momentum * vw2[j][l];
                    vw2[j][l] = delta_w;
                    w2[j][l] += delta_w;
                }
            }

            for (j = 1; j <= N_OCULTOS-1; j++)
            { // quantidade de saidas de neuronios (sem bias)
                for (i = 1; i <= N_ENTRADAS; i++)
                { // neuronios na camada
                    float delta_w = learning_rate * d1[j] * O0[i] + momentum * vw1[i][j];
                    vw1[i][j] = delta_w;
                    w1[i][j] += delta_w;
                }
            }

            er += (t1[cs] - O2[1]) * (t1[cs] - O2[1]) + (t2[cs] - O2[2]) * (t2[cs] - O2[2]) + (t3[cs] - O2[3]) * (t3[cs] - O2[3]) + (t4[cs] - O2[4]) * (t4[cs] - O2[4]) + (t5[cs] - O2[5]) * (t5[cs] - O2[5]) + (t6[cs] - O2[6]) * (t6[cs] - O2[6]) + (t7[cs] - O2[7]) * (t7[cs] - O2[7]);
        }
        er = er / 15000.0;
        if (m % 100 == 0) {
            printf("Epoca %d - Erro: %f \n", m, er);
        }
    }

    printf("\nAvaliando modelo no conjunto de treinamento...\n");
    int acertos_treino = 0;
    float loss_treino_mse = 0.0;
    float loss_treino_cross_entropy = 0.0;
    
    for (cs = 0; cs < total_samples_treino; cs++)
    {
        I0[1] = E1[cs];
        I0[2] = E2[cs];
        I0[3] = E3[cs];
        I0[4] = E4[cs];
        I0[5] = E5[cs];
        I0[6] = E6[cs];
        I0[N_ENTRADAS] = 1.0;

        for (i = 1; i <= N_ENTRADAS; i++)
            O0[i] = I0[i];

        for (j = 1; j <= N_OCULTOS-1; j++)
        { // quantidade de saidas de neuronios (sem bias)
            I1[j] = 0.0;
            for (i = 1; i <= N_ENTRADAS; i++)
            { // neuronios na camada
                I1[j] += O0[i] * w1[i][j];
            }
            O1[j] = 1.0 / (1.0 + exp(-I1[j]));
        }

        O1[N_OCULTOS] = 1.0;

        for (l = 1; l <= N_SAIDAS; l++)
        { // quantidade de saidas de neuronios
            I2[l] = 0.0;
            for (j = 1; j <= N_OCULTOS; j++)
            { // neuronios na camada
                I2[l] += O1[j] * w2[j][l];
            }
            O2[l] = 1.0 / (1.0 + exp(-I2[l]));
        }

        int pred = 1;
        for (int k = 2; k <= N_SAIDAS; k++)
        {
            if (O2[k] > O2[pred])
                pred = k;
        }

        int real = 1;
        if (t2[cs] == 1.0)
            real = 2;
        if (t3[cs] == 1.0)
            real = 3;
        if (t4[cs] == 1.0)
            real = 4;
        if (t5[cs] == 1.0)
            real = 5;
        if (t6[cs] == 1.0)
            real = 6;
        if (t7[cs] == 1.0)
            real = 7;

        // Calcular acurácia
        if (pred == real)
            acertos_treino++;

        // Calcular loss MSE
        loss_treino_mse += (t1[cs] - O2[1]) * (t1[cs] - O2[1]) + 
                          (t2[cs] - O2[2]) * (t2[cs] - O2[2]) + 
                          (t3[cs] - O2[3]) * (t3[cs] - O2[3]) + 
                          (t4[cs] - O2[4]) * (t4[cs] - O2[4]) + 
                          (t5[cs] - O2[5]) * (t5[cs] - O2[5]) + 
                          (t6[cs] - O2[6]) * (t6[cs] - O2[6]) + 
                          (t7[cs] - O2[7]) * (t7[cs] - O2[7]);

        // Calcular loss Cross-Entropy
        loss_treino_cross_entropy += log(t1[cs] * O2[1] + t2[cs] * O2[2] + t3[cs] * O2[3] + 
                                         t4[cs] * O2[4] + t5[cs] * O2[5] + t6[cs] * O2[6] + 
                                         t7[cs] * O2[7] + 1e-15); // epsilon para evitar log(0)
    }

    loss_treino_mse /= total_samples_treino;
    loss_treino_cross_entropy = -loss_treino_cross_entropy / total_samples_treino;

    printf("\nAvaliando modelo no conjunto de teste...\n");

    int acertos = 0;
    float loss_teste_mse = 0.0;
    float loss_teste_cross_entropy = 0.0;
    
    for (cs = 0; cs < total_samples_teste; cs++)
    {
        I0[1] = E1_test[cs];
        I0[2] = E2_test[cs];
        I0[3] = E3_test[cs];
        I0[4] = E4_test[cs];
        I0[5] = E5_test[cs];
        I0[6] = E6_test[cs];
        I0[N_ENTRADAS] = 1.0; // bias

        for (i = 1; i <= N_ENTRADAS; i++)
            O0[i] = I0[i];

        for (j = 1; j <= N_OCULTOS-1; j++)
        { // quantidade de saidas de neuronios (sem bias)
            I1[j] = 0.0;
            for (i = 1; i <= N_ENTRADAS; i++)
            { // neuronios na camada
                I1[j] += O0[i] * w1[i][j];
            }
            O1[j] = 1.0 / (1.0 + exp(-I1[j]));
        }

        O1[N_OCULTOS] = 1.0; // bias

        for (l = 1; l <= N_SAIDAS; l++)
        { // quantidade de saidas de neuronios
            I2[l] = 0.0;
            for (j = 1; j <= N_OCULTOS; j++)
            { // neuronios na camada
                I2[l] += O1[j] * w2[j][l];
            }
            O2[l] = 1.0 / (1.0 + exp(-I2[l]));
        }

        int pred = 1;
        for (int k = 2; k <= N_SAIDAS; k++)
        {
            if (O2[k] > O2[pred])
                pred = k;
        }

        int real = 1;
        if (t2_test[cs] == 1.0)
            real = 2;
        if (t3_test[cs] == 1.0)
            real = 3;
        if (t4_test[cs] == 1.0)
            real = 4;
        if (t5_test[cs] == 1.0)
            real = 5;
        if (t6_test[cs] == 1.0)
            real = 6;
        if (t7_test[cs] == 1.0)
            real = 7;

        // Calcular acurácia
        if (pred == real)
            acertos++;

        // Calcular loss MSE
        loss_teste_mse += (t1_test[cs] - O2[1]) * (t1_test[cs] - O2[1]) + 
                         (t2_test[cs] - O2[2]) * (t2_test[cs] - O2[2]) + 
                         (t3_test[cs] - O2[3]) * (t3_test[cs] - O2[3]) + 
                         (t4_test[cs] - O2[4]) * (t4_test[cs] - O2[4]) + 
                         (t5_test[cs] - O2[5]) * (t5_test[cs] - O2[5]) + 
                         (t6_test[cs] - O2[6]) * (t6_test[cs] - O2[6]) + 
                         (t7_test[cs] - O2[7]) * (t7_test[cs] - O2[7]);

        // Calcular loss Cross-Entropy
        loss_teste_cross_entropy += log(t1_test[cs] * O2[1] + t2_test[cs] * O2[2] + t3_test[cs] * O2[3] + 
                                       t4_test[cs] * O2[4] + t5_test[cs] * O2[5] + t6_test[cs] * O2[6] + 
                                       t7_test[cs] * O2[7] + 1e-15);

        if (cs < 11)
        {
            printf("Teste %d -> Saidas: [%.3f %.3f %.3f %.3f %.3f %.3f %.3f]  Previsto=%d Real=%d\n",
               cs, O2[1], O2[2], O2[3], O2[4], O2[5], O2[6], O2[7], pred, real);
        }

    }

    loss_teste_mse /= total_samples_teste;
    loss_teste_cross_entropy = -loss_teste_cross_entropy / total_samples_teste;

    printf("\n=== RESUMO FINAL ===\n");

    printf("TREINAMENTO: Acuracia=%.2f%% | MSE=%.6f | Cross-Entropy=%.6f\n", 
           (acertos_treino / (float)total_samples_treino) * 100.0, 
           loss_treino_mse, loss_treino_cross_entropy);
    printf("TESTE:       Acuracia=%.2f%% | MSE=%.6f | Cross-Entropy=%.6f\n", 
           (acertos / (float)total_samples_teste) * 100.0, 
           loss_teste_mse, loss_teste_cross_entropy);

    float diferenca = (acertos_treino / (float)total_samples_treino) - (acertos / (float)total_samples_teste);
    printf("Diferenca (Overfitting): %.2f%%\n", diferenca * 100.0);
    
    if (diferenca > 0.1) {
        printf("ALERTA: Possivel overfitting detectado!\n");
    } else if (diferenca < 0.02) {
        printf("Modelo bem generalizado.\n");
    } else {
        printf("Generalizacao aceitavel.\n");
    }

    fim = clock();
    tempo_exec = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("\nTempo total de execucao: %.3f segundos\n", tempo_exec);

    getch();
}
