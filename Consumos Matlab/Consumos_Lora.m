clear all;
clc;

pir = 18.6e-6;
lora_receive = 0.094;
lora_transmit = 0.015;
deep_sleep = 30e-6;

tempo_rx = 0.5;
tempo_tx = 1;
tempo_ativo_rx = tempo_rx/3600;
tempo_ativo_tx = tempo_tx/3600;
tempo_ativo = tempo_ativo_rx + tempo_ativo_tx;

i = 1;

bateria = 2
for k = 1:1:20
    consumo_pir = pir*24;
    consumo_lora_tx(k) = lora_transmit*24*tempo_ativo_tx*k;
    consumo_lora_rx(k) = lora_receive*24*tempo_ativo_tx*k;
    consumo_lora_deep = deep_sleep*24*(1-2*tempo_ativo);
    
    consumo_total_deep = consumo_pir+consumo_lora_rx+consumo_lora_tx+consumo_lora_deep;
    
 end
    
autonomia_deep2 = bateria./consumo_total_deep;

k1 = 1:1:20;
figure(1)
plot(k1,autonomia_deep2,'r');
hold on
bateria2 = 4
for k = 1:1:20
    consumo_pir = pir*24;
    consumo_lora_tx(k) = lora_transmit*24*tempo_ativo_tx*k;
    consumo_lora_rx(k) = lora_receive*24*tempo_ativo_tx*k;
    consumo_lora_deep = deep_sleep*24*(1-2*tempo_ativo);
    
    consumo_total_deep = consumo_pir+consumo_lora_rx+consumo_lora_tx+consumo_lora_deep;
    
 end
    
autonomia_deep4 = bateria2./consumo_total_deep;
k2 = 1:1:20;
plot(k2,autonomia_deep4,'b');
hold on

bateria5 = 5;
for k = 1:1:20
    consumo_pir = pir*24;
    consumo_lora_tx(k) = lora_transmit*24*tempo_ativo_tx*k;
    consumo_lora_rx(k) = lora_receive*24*tempo_ativo_tx*k;
    consumo_lora_deep = deep_sleep*24*(1-2*tempo_ativo);
    
    consumo_total_deep = consumo_pir+consumo_lora_rx+consumo_lora_tx+consumo_lora_deep;
    
 end
    
autonomia_deep5 = bateria5./consumo_total_deep;

k3 = 1:1:20;
figure(1)
plot(k3,autonomia_deep5,'g');
hold on

bateria10 = 10;
for k = 1:1:20
    consumo_pir = pir*24;
    consumo_lora_tx(k) = lora_transmit*24*tempo_ativo_tx*k;
    consumo_lora_rx(k) = lora_receive*24*tempo_ativo_tx*k;
    consumo_lora_deep = deep_sleep*24*(1-2*tempo_ativo);
    
    consumo_total_deep = consumo_pir+consumo_lora_rx+consumo_lora_tx+consumo_lora_deep;
    
 end
    
autonomia_deep10 = bateria10./consumo_total_deep;

k4 = 1:1:20;
figure(1)
plot(k4,autonomia_deep10,'c');

hold off
legend("Deep sleep e 2 Ah","Deep sleep e 4 Ah","Deep sleep e 5 Ah","Deep sleep e 10 Ah" );
title("Consumo Lopy4 com diferentes baterias")
ylabel("Autonomia (dias)");
xlabel("N�mero de leituras por dia");