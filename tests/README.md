# Documentație Teste - Proiect FixItNow

Acest folder conține scenarii de test utilizate pentru verificarea funcționalității aplicației, validarea datelor de intrare și demonstrarea logicii de simulare.

Fisierele sunt în format text și pot fi încărcate din meniul principal al aplicației (Opțiunea 1).

## Descrierea Fișierelor

### 1. `test1.txt` (Scenariu Ideal)
* **Descriere:** Acest fișier conține exclusiv informații corecte (angajați valizi, modele valide, cereri corecte).
* **Scop:** Este conceput pentru a demonstra funcționarea corectă a simulării într-un caz ideal.
* **Ce se urmărește:** Se poate observa algoritmul de distribuire a cererilor: sistemul va aloca cererile în mod egal către tehnicieni, încercând să mențină încărcarea (timpul total de lucru) echilibrată între aceștia.

### 2. `test2.txt` (Erori Angajați - Validare Input & Restricții Service)
* **Descriere:** Conține încercări de a introduce angajați cu date invalide:
    * CNP eronat (lungime greșită sau cifră de control invalidă).
    * Nume invalide (prea scurte).
    * Vârstă sub limita legală (sub 16 ani, calculat din CNP).
* **Rezultat Așteptat:** Aplicația va afișa erori specifice pentru fiecare linie greșită.
* **Impact:** Din cauza numărului mare de angajați respinși, service-ul nu va atinge numărul minim de personal necesar (3 Tehnicieni, 1 Recepționer, 1 Supervizor). Prin urmare, **simularea procesării cererilor nu va putea începe**, demonstrând mecanismul de siguranță al aplicației.

### 3. `test3.txt` (Erori Cereri)
* **Descriere:** Scenariu axat pe validarea datelor din cererile de reparație. Conține:
    * Cereri pentru modele de electrocasnice care nu există în catalogul service-ului.
    * Cereri cu grad de complexitate 0, sau invalid (mai mare de 5).
* **Rezultat Așteptat:** Aplicația va detecta și va respinge cererile invalide, afișând mesaje de eroare ("Cerere respinsă"), dar va continua să încarce cererile valide. Aceste cereri invalide vor fi contorizate în statistica de "Aparate nereparate".

### 4. `test4.txt` (Scenariu "Low Load")
* **Descriere:** Conține o listă extinsă de angajați valizi, dar un număr redus de cereri de reparație.
* **Scop:** Verifică comportamentul sistemului când resursele umane depășesc volumul de muncă.
* **Ce se urmărește:** Se verifică modul in care cererile sunt distribuite catre tehincieni, atunci caănd acesștia depășesc numarul de cereri