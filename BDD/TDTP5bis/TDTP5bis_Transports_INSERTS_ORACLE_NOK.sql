--------------------------------------------------------------------------------------------
--Contraintes avancées qu'on ne peut pas contrôler avec des contraintes d'intégrité
--Doc Triggers ORACLE ici : https://docs.oracle.com/cd/E11882_01/appdev.112/e25519/create_trigger.htm#LNPLS01374
--Doc Trigger SQlite ici : https://www.sqlite.org/lang_createtrigger.html
--Exemples SQLite ici : https://youtu.be/i3xwZw5VRMs
--Explications SQLite ici : https://youtu.be/s9iRD-zrxS0
--------------------------------------------------------------------------------------------
    -- [NOK] (on n'accepte pas dateDep < dateAchat pour un Camion)
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES (103, TO_DATE('2006-09-16', 'YYYY-MM-DD'), TO_DATE('2006-09-17', 'YYYY-MM-DD'), 'Lyon', 'Grenoble', 1002, '18 CHX 38', 1);
-- [NOK] (on n'accepte pas l'insertion du chauffeur noP=4 si existe deja une secretaire avec noP=4)
INSERT INTO LesChauffeurs (noP, nbK) VALUES (4, 1);
-- [NOK] (on n'accepte pas l'insertion d'une secretaire noP=3 si existe deja un chauffeur avec noP=3)
INSERT INTO LesSecretaires (noP, noC) VALUES (3, 1004);
-- [NOK] (on n'accepte pas un chevauchement des dates pour un chauffeur)
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES (104, TO_DATE('2017-02-01', 'YYYY-MM-DD'),  TO_DATE('2017-02-28', 'YYYY-MM-DD'), 'Grenoble', 'Marseille', 1002, '1675 WZ 38', 3);
-- [NOK] (on n'accepte pas un chevauchement des dates pour un camion)
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
   VALUES (105, TO_DATE('2017-01-02', 'YYYY-MM-DD'), TO_DATE('2017-02-20', 'YYYY-MM-DD'), 'Lyon', 'Marseille', 1001, '13 CHX 38', 1);


   
