CREATE TABLE `dbtify`.`album_artist` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `album_id` INT NOT NULL,
  `artist_id` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `album_idx` (`album_id` ASC) VISIBLE,
  INDEX `artist_idx` (`artist_id` ASC) VISIBLE,

    FOREIGN KEY (`album_id`)
    REFERENCES `dbtify`.`album` (`id`)
    ON DELETE CASCADE,

    FOREIGN KEY (`artist_id`)
    REFERENCES `dbtify`.`artist` (`id`)
    ON DELETE CASCADE);
