DROP TABLE LesContrats;
DROP TABLE LesDistances;
DROP TABLE LesChauffeurs;
DROP TABLE LesSecretaires;
DROP TABLE LesCamions;
DROP TABLE LesClients;
DROP TABLE LesPersonnes;

CREATE TABLE LesPersonnes(
    noP NUMBER (4,0),
    nom VARCHAR(100),
    prenom VARCHAR(100),
    salaire NUMBER (8,2),
    CONSTRAINT p_pk PRIMARY KEY (noP),
    CONSTRAINT p_ch1 CHECK (noP > 0),
    CONSTRAINT p_ch2 CHECK (salaire >= 0)
);

CREATE TABLE LesClients(
    noC NUMBER (4,0),
    nom VARCHAR(100),
    adresse VARCHAR(100),
    CONSTRAINT c_pk PRIMARY KEY (noC),
    CONSTRAINT c_ch CHECK (noC > 0)
);

CREATE TABLE LesCamions(
    immat VARCHAR(100),
    marque VARCHAR(100),
    dateAchat DATE,
    CONSTRAINT ca_pk PRIMARY KEY (immat)
);

CREATE TABLE LesSecretaires(
    noP NUMBER (4,0),
    noC NUMBER (4,0),
    CONSTRAINT s_pk PRIMARY KEY (noC),
    CONSTRAINT S_fk1 FOREIGN KEY (noP) REFERENCES LesPersonnes(noP) ON DELETE CASCADE,
    CONSTRAINT S_fk2 FOREIGN KEY (noC) REFERENCES LesClients(noC)
);

CREATE TABLE LesChauffeurs(
    noP NUMBER (4,0),
    nbK NUMBER (8,0),
    CONSTRAINT ch_pk PRIMARY KEY (noP),
    CONSTRAINT ch_fk FOREIGN KEY (noP) REFERENCES LesPersonnes(noP) ON DELETE CASCADE
);

CREATE TABLE LesDistances (
    vDep VARCHAR(100),
    vArr VARCHAR(100),
    nbK NUMBER(6),
    CONSTRAINT d_pk PRIMARY KEY (vDep,vArr),
    CONSTRAINT d_ch CHECK (nbK >= 0)
);

CREATE TABLE LesContrats(
    noTr NUMBER (4,0),
    dateDep DATE,
    dateArr DATE,
    vDep VARCHAR(100),
    vArr  VARCHAR(100),
    noC NUMBER (4,0),
    immat VARCHAR(100),
    noP NUMBER (4,0),
    CONSTRAINT co_pk PRIMARY KEY (noTr),
    CONSTRAINT co_fk1 FOREIGN KEY (immat) REFERENCES LesCamions(immat),
    CONSTRAINT co_fk2 FOREIGN KEY (noC) REFERENCES LesClients(noC),
    CONSTRAINT co_fk3 FOREIGN KEY (noP) REFERENCES LesChauffeurs(noP),
    CONSTRAINT co_fk4 FOREIGN KEY (vDep,vArr) REFERENCES LesDistances(vDep,vArr),
    CONSTRAINT co_ch1 CHECK (dateDep <= dateArr)
);

------------------------------------------------------------
------- INSERTS --------------------------------------------
------------------------------------------------------------

INSERT INTO LesDistances (vDep, vArr, nbK) VALUES ('Lyon', 'Marseille', 314);
INSERT INTO LesDistances (vDep, vArr, nbK) VALUES ('Lyon', 'Paris', 466);
INSERT INTO LesDistances (vDep, vArr, nbK) VALUES ('Lyon', 'Grenoble', 112);
INSERT INTO LesDistances (vDep, vArr, nbK) VALUES ('Marseille', 'Grenoble', 306);
INSERT INTO LesDistances (vDep, vArr, nbK) VALUES ('Marseille', 'Lyon', 314);
INSERT INTO LesDistances (vDep, vArr, nbK) VALUES ('Marseille', 'Paris', 774);
INSERT INTO LesDistances (vDep, vArr, nbK) VALUES ('Grenoble', 'Marseille', 306);
INSERT INTO LesDistances (vDep, vArr, nbK) VALUES ('Grenoble', 'Paris', 574);
INSERT INTO LesDistances (vDep, vArr, nbK) VALUES ('Grenoble', 'Lyon', 112);
INSERT INTO LesDistances (vDep, vArr, nbK) VALUES ('Paris', 'Marseille', 774);
INSERT INTO LesDistances (vDep, vArr, nbK) VALUES ('Paris', 'Lyon', 466);
INSERT INTO LesDistances (vDep, vArr, nbK) VALUES ('Paris', 'Grenoble', 574);

INSERT INTO LesPersonnes (noP, nom, prenom, salaire) VALUES (1, 'Peyrin', 'Jean-Pierre', 68);
INSERT INTO LesPersonnes (noP, nom, prenom, salaire) VALUES (2, 'Fauvet', 'Marie-Christine', 140);
INSERT INTO LesPersonnes (noP, nom, prenom, salaire) VALUES (3, 'Bouchenak', 'Sara', 140);
INSERT INTO LesPersonnes (noP, nom, prenom, salaire) VALUES (4, 'Tonton', 'Michael', 160);
INSERT INTO LesPersonnes (noP, nom, prenom, salaire) VALUES  (5, 'L''enchanteur', 'Merlin', 160);
INSERT INTO LesPersonnes (noP, nom, prenom, salaire) VALUES (99, 'Adrian', 'Laporte', 180);

INSERT INTO LesClients (noC, nom, adresse) VALUES (1001, 'Mickey Mouse', 'Echirolles');
INSERT INTO LesClients (noC, nom, adresse) VALUES (1002, 'Tintin', 'Bruxelles');
INSERT INTO LesClients (noC, nom, adresse) VALUES (1003, 'Rantanplan', 'Washington');
INSERT INTO LesClients (noC, nom, adresse) VALUES (1004, 'Mafalda', 'Gap');


INSERT INTO LesCamions (immat, marque, dateAchat) VALUES ('1675 WZ 38', 'renault', TO_DATE('2004-05-01', 'YYYY-MM-DD'));
INSERT INTO LesCamions (immat, marque, dateAchat) VALUES ('1 A 38', 'renault', TO_DATE('2005-01-01', 'YYYY-MM-DD'));
INSERT INTO LesCamions (immat, marque, dateAchat) VALUES ('13 CHX 38', 'renault', TO_DATE('2016-07-14', 'YYYY-MM-DD'));
INSERT INTO LesCamions (immat, marque, dateAchat) VALUES ('18 CHX 38', 'renault', TO_DATE('2017-07-14', 'YYYY-MM-DD'));
INSERT INTO LesCamions (immat, marque, dateAchat) VALUES ('20 CHX 38', 'renault', TO_DATE('2017-07-14', 'YYYY-MM-DD'));


INSERT INTO LesSecretaires (noP, noC) VALUES (2, 1001);
INSERT INTO LesSecretaires (noP, noC) VALUES (2, 1002);
INSERT INTO LesSecretaires (noP, noC) VALUES (4, 1003);

-- NBKM 0 PAR DEFAUT
INSERT INTO LesChauffeurs (noP) VALUES (1);
INSERT INTO LesChauffeurs (noP) VALUES (3);
INSERT INTO LesChauffeurs (noP) VALUES (5);

INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
VALUES (100, TO_DATE('2017-01-01', 'YYYY-MM-DD'), TO_DATE('2017-02-21', 'YYYY-MM-DD'), 'Lyon', 'Marseille', 1001, '13 CHX 38', 3);
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
VALUES (101, TO_DATE('2018-12-05', 'YYYY-MM-DD'), TO_DATE('2019-01-01', 'YYYY-MM-DD'), 'Marseille', 'Paris', 1001, '1 A 38', 3);
INSERT INTO LesContrats (noTr, dateDep, dateArr, vDep, vArr, noC, immat, noP)
VALUES  (102, TO_DATE('2016-08-05', 'YYYY-MM-DD'), TO_DATE('2017-08-17', 'YYYY-MM-DD'), 'Lyon', 'Paris', 1002, '1675 WZ 38', 5);