float tarifa_estimativa(int distancia_m) {
  float tarifa_base = 2.0f;
  float valor_distancia = (0.4f * (float)distancia_m) / 1000.0f;
  float valor_previsto = tarifa_base + valor_distancia;
  return valor_previsto;
}

float tarifa_final(int distancia_m, float duracao_s) {
  float valor_final = tarifa_estimativa(distancia_m);
  // Adiciona taxa de tempo conforme a duração
  if (duracao_s >= 0 && duracao_s < 180) {
    valor_final += 1.5f;
  }
  return valor_final;
}