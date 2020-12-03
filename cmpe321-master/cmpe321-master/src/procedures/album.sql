CREATE TABLE `dbtify`.`album` (
  `id` INT NOT NULL,
  `genre` VARCHAR(200) NOT NULL,
  `title` VARCHAR(200) NOT NULL,
  `artist_id` INT NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE,
  INDEX `artist_idx` (`artist_id` ASC) VISIBLE,

    FOREIGN KEY (`artist_id`)
    REFERENCES `dbtify`.`artist` (`id`)
    ON DELETE CASCADE);

