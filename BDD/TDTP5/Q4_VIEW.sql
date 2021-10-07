DROP VIEW Chauffeurs;

CREATE VIEW Chauffeurs(noP,nbK) AS
    SELECT noP, SUM(LesDistances.nbK)
    FROM LesContrats JOIN LesChauffeurs USING(noP) JOIN LesDistances USING(vDep,vArr)
    GROUP BY noP;