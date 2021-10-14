-- Trigger secrétaires doublon noP avec chauffeurs
CREATE OR REPLACE TRIGGER LesSecretaires_doublon
    BEFORE INSERT ON LesSecretaires
    FOR EACH ROW
    DECLARE
        countNOP INT;
        doublonNOP EXCEPTION;
    BEGIN
        SELECT COUNT(*) INTO countNOP
        FROM LesChauffeurs C
        WHERE C.noP = :NEW.noP;
        IF countNOP > 0 THEN
            RAISE doublonNOP;
        END IF;
        EXCEPTION
            WHEN doublonNOP THEN
                raise_application_error(-20001, 'Cannot insert noP with LesSecretaires.noP == LesChauffeurs.noP');
    END;


-- Trigger contrats chevauchement des dates pour un chauffeur
CREATE OR REPLACE TRIGGER LesContrats_chevauchement
    BEFORE INSERT ON LesContrats
    FOR EACH ROW
    WHEN (NEW.noP = OLD.noP)
    DECLARE
        noPchauffeurs INT;
        DateDepart DATE;
        DateArrivee DATE;
        chevauchementDates EXCEPTION;
    BEGIN
        SELECT noP,dateDep,dateArr INTO noPchauffeurs,DateDepart,DateArrivee
        FROM LesContrats;
        IF :NEW.noP = noPchauffeurs AND ((:NEW.dateDep <= DateDepart AND :NEW.dateArr <= DateArrivee) OR (:NEW.dateDep >= DateDepart AND :NEW.dateArr >= DateArrivee) OR (:NEW.dateDep <= DateDepart AND :NEW.dateArr >= DateArrivee) OR (:NEW.dateDep >= DateDepart AND :NEW.dateArr <= DateArrivee)) THEN
            RAISE chevauchementDates;
        END IF;
        EXCEPTION
            WHEN chevauchementDates THEN
                raise_application_error(-20001, 'Cannot insert this contrat with :NEW.dateArr,dateDep ⊂ :OLD.dateArr,DateDep and :NEW.noP = OLD.noP');
    END;