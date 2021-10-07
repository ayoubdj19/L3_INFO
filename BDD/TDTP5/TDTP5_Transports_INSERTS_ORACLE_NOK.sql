-- Chaque ligne de ce fichier devrait produire un erreur

---------------------------------------------------------------------------------------------
---------------------------------- Contraintes à verifier -----------------------------------
---------------------------------------------------------------------------------------------
--NOK (on n'accepte pas des personnes avec meme noP)
INSERT INTO LesPersonnes (noP, nom, prenom, salaire) VALUES (1, 'Pierre', 'Dupont', 111);
--NOK (on n'accepte pas des personnes sans nom et prenom)
INSERT INTO LesPersonnes (noP, nom, prenom, salaire) VALUES (7, 'Pedro', NULL, 40);
INSERT INTO LesPersonnes (noP, nom, prenom, salaire) VALUES (8, NULL, 'Martinez', 40);
--NOK (on n'accepte pas des secretaires qui ne sont pas dans la relation LesPersonnes)
INSERT INTO LesSecretaires (noP, noC) VALUES (8, 1004);
--NOK (on n'accepte pas des secretaires lies a un client qui n'est pas repertorie dans la relation LesClients)
INSERT INTO LesSecretaires (noP, noC) VALUES (2, 1006);
--NOK (on n'accepte pas des clients qui sont sous la responsabilite de differentes secretaires)
INSERT INTO LesSecretaires (noP, noC) VALUES (2, 1001);
--NOK (on n'accepte pas des clients sans secretaire)
INSERT INTO LesSecretaires (noP, noC) VALUES (NULL, 1007);

--NOK (on n'accepte pas un nombre de Km negatif)
INSERT INTO LesChauffeurs (noP, nbK) VALUES (99, -20);
--NOK (on n'accepte pas des chauffeurs sans nbKm)
INSERT INTO LesChauffeurs (noP, nbK) VALUES (99, NULL);

--NOK (on n'accepte pas des secretaires comme chauffeurs dans un contrat)
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES  (107, TO_DATE('2016-08-04', 'YYYY-MM-DD'), TO_DATE('2017-08-05', 'YYYY-MM-DD'), 'Lyon', 'Paris', 1002, '1675 WZ 38', 2);
--NOK (on n'accepte que des camions repertories dans la relation LesCamions)
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES  (108, TO_DATE('2016-08-04', 'YYYY-MM-DD'), TO_DATE('2017-08-05', 'YYYY-MM-DD'), 'Lyon', 'Paris', 1002, '1675 WZ 39', 1);

--NOK (on n'accepte pas des valeurs NULL dans les dates, les villes et le numero de client)
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES  (109, NULL, TO_DATE('2016-08-04', 'YYYY-MM-DD'), 'Lyon', 'Paris', 1002, '1675 WZ 38', 3);
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES  (110, TO_DATE('2016-08-04', 'YYYY-MM-DD'), NULL, 'Lyon', 'Paris', 1002, '1675 WZ 38', 3);
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES  (111, TO_DATE('2016-08-04', 'YYYY-MM-DD'), TO_DATE('2016-08-06', 'YYYY-MM-DD'), NULL, 'Paris', 1002, '1675 WZ 38', 3);
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES  (112, TO_DATE('2016-08-04', 'YYYY-MM-DD'), TO_DATE('2016-08-06', 'YYYY-MM-DD'), 'Lyon', NULL, 1002, '1675 WZ 38', 3);

--NOK (on n'accepte pas des chauffeurs qui partent/arrivent a la même date sur un autre contrat le même jour)
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES  (113, TO_DATE('2016-08-05', 'YYYY-MM-DD'), TO_DATE('2017-08-17', 'YYYY-MM-DD'), 'Lyon', 'Paris', 1002, '13 CHX 38', 5);
--NOK (on n'accepte pas des camions qui partent/arrivent a la meme date sur un autre contrat le même jour)
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES  (114, TO_DATE('2016-08-04', 'YYYY-MM-DD'), TO_DATE('2017-08-17', 'YYYY-MM-DD'), 'Lyon', 'Paris', 1002, '1675 WZ 38', 1);

--NOK (on n'accepte pas des departs et arrivees sur la meme ville)
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES  (115, TO_DATE('2016-08-04', 'YYYY-MM-DD'), TO_DATE('2016-08-05', 'YYYY-MM-DD'), 'Lyon', 'Lyon', 1002, '1675 WZ 38', 1);
--NOK (on n'accepte pas une date d'arrivee plus recente qu'une date de depart sur un contrat)
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES  (116, TO_DATE('2016-08-05', 'YYYY-MM-DD'), TO_DATE('2016-08-04', 'YYYY-MM-DD'), 'Lyon', 'Paris', 1002, '1675 WZ 38', 5);

----------------------------------------------------------------------------------------------
--Créer une vue LesChauffeurs en fonction des distances parcourues selon ses contrats.
--Au lieu de garder nbK de manière statique on va le calculer dynamiquement grâce à une vue.
--Résultat attendu:
--TABLE LesChauffeurs_base(noP)
--VIEW LesChauffeurs(noP, nbK)
----------------------------------------------------------------------------------------------