DROP VIEW LesChauffeurs;
CREATE VIEW LesChauffeurs (noP, nbK) AS
    WITH nbKmsContrat AS (
        SELECT noTr, nbK as distanceContrat, noP
        FROM LesContrats JOIN LesDistances USING (vDep, vArr)
    )
    SELECT noP, COALESCE(SUM(distanceContrat),0) as nbKTotal
    FROM LesChauffeurs_base LEFT OUTER JOIN nbKmsContrat USING (noP)
    GROUP BY noP;