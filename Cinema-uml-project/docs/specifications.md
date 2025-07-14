# Specifications - Online Cinema System

## 1. User Roles

### 1.1 Guest
- Can browse movie catalog
- Can register an account
- Can log in

### 1.2 Registered User (Viewer)
- Can log in/log out
- Can browse and filter movies
- Can purchase or rent movies
- Can watch purchased/rented movies
- Can manage their profile
- Can leave reviews for movies

### 1.3 Administrator
- Can log in
- Can add, edit, delete movies
- Can manage users (view, block)
- Can view system statistics

---

## 2. Functional Requirements

### 2.1 Authentication and Authorization
- Users must register and log in to purchase or rent content
- Admin panel accessible only to users with admin role

### 2.2 Movie Catalog
- Movies can be filtered by genre, rating, release year
- Each movie contains: title, description, genre(s), duration, release date, rating, preview image

### 2.3 Movie Purchase / Rental
- Registered users can:
  - Purchase a movie (permanent access)
  - Rent a movie (temporary access with expiration)
- System must:
  - Validate access rights before playback
  - Handle payment processing securely

### 2.4 Video Streaming
- Only users with valid purchase/rental can stream
- Support for secure video delivery (HLS/DASH or DRM)

### 2.5 Profile Management
- Users can update profile data (name, email, password)
- View history of purchases/rentals

### 2.6 Admin Features
- Add/edit/delete movies
- View registered users
- View overall statistics (sales, top movies)

---

## 3. Non-Functional Requirements

- **Scalability**: system must support hundreds of concurrent streams
- **Security**: encrypted passwords, secure payments, access control
- **Availability**: 99.9% uptime expected
- **Performance**: movie playback should start within 3 seconds
- **Responsiveness**: UI must be mobile-friendly

---

## 4. Data Model Overview

### Entities:
- `User`: id, name, email, passwordHash, role
- `Movie`: id, title, genre, description, videoUrl, pricePurchase, priceRental
- `Purchase`: id, userId, movieId, type, date, expirationDate
- `Review`: id, userId, movieId, rating, comment
- `PaymentMethod`: id, userId, type, token

---

## 5. Business Rules

- A rental is valid for 48 hours from the moment of activation
- A user cannot rent or purchase the same movie multiple times simultaneously
- Reviews can only be left for purchased or rented movies
- Admins cannot be deleted from the system

---

## 6. Future Extensions (Optional)
- Subscription model (monthly access)
- Recommendation engine
- Watch party / group viewing
- Multi-language subtitles and dubbing
