-- Cette table garde tout changement dans les salaires des personnes.
-- DROP TABLE Salaires_Personnes_log;
CREATE TABLE Salaires_Personnes_log (
   log_id NUMBER (4,0) DEFAULT 0,
   noP NUMBER (4,0) NOT NULL,
   salaire_old NUMBER (8,2),
   salaire_new NUMBER (8,2),
   date_modification DATE,
   CONSTRAINT fk_salaires_log__noP FOREIGN KEY (noP) REFERENCES LesPersonnes(noP)
);
------------------------------------------------------------------------------
-- Création d'un mécanisme d'incrémentation automatique de l'id (Oracle 11.2)
-- AUTOINCREMENT sur d'autre systèmes comme SQlite ou MySQL
------------------------------------------------------------------------------
-- Céation de la constrainte clé primaire
ALTER TABLE Salaires_Personnes_log ADD (
CONSTRAINT log_pk PRIMARY KEY (log_id));

-- Création d'une séquence à partir de 1 
-- DROP SEQUENCE  log_pk;
CREATE SEQUENCE log_pk START WITH 1;

-- Création d'un trigger pour incrément automatique du log_id
CREATE OR REPLACE TRIGGER log_autoincrement
BEFORE INSERT ON Salaires_Personnes_log
FOR EACH ROW
BEGIN
   SELECT log_pk.NEXTVAL
   INTO   :NEW.log_id
   FROM   dual;
END;
/


   
