# Notification Server API Documentation

## Overview
The Notification Server provides a comprehensive API for managing notifications, templates, and delivery analytics. All API endpoints follow REST conventions and return JSON responses.

## Base URL
```
http://localhost:8080/api/v1
```

## Authentication
All API requests require a valid JWT token in the Authorization header:
```
Authorization: Bearer <jwt_token>
```

## Notification Management

### Create Notification
```
POST /notifications
```

**Request Body:**
```json
{
  "title": "string",
  "content": "string",
  "type": "INFO|WARNING|ERROR|SUCCESS",
  "priority": "LOW|NORMAL|HIGH|CRITICAL",
  "recipient_type": "USER|GROUP|ALL",
  "recipient_id": "string",
  "sender_id": "string",
  "template_id": "string (optional)",
  "payload": "object (optional)",
  "scheduled_at": "ISO 8601 timestamp (optional)",
  "expires_at": "ISO 8601 timestamp (optional)"
}
```

**Response:**
```json
{
  "id": "string",
  "title": "string",
  "content": "string",
  "type": "string",
  "priority": "string",
  "recipient_type": "string",
  "recipient_id": "string",
  "sender_id": "string",
  "created_at": "ISO 8601 timestamp",
  "updated_at": "ISO 8601 timestamp"
}
```

### Get Notification
```
GET /notifications/{id}
```

**Response:**
```json
{
  "id": "string",
  "title": "string",
  "content": "string",
  "type": "string",
  "priority": "string",
  "recipient_type": "string",
  "recipient_id": "string",
  "sender_id": "string",
  "template_id": "string",
  "payload": "object",
  "scheduled_at": "ISO 8601 timestamp",
  "expires_at": "ISO 8601 timestamp",
  "created_at": "ISO 8601 timestamp",
  "updated_at": "ISO 8601 timestamp"
}
```

### Update Notification
```
PUT /notifications/{id}
```

**Request Body:**
```json
{
  "title": "string",
  "content": "string",
  "type": "INFO|WARNING|ERROR|SUCCESS",
  "priority": "LOW|NORMAL|HIGH|CRITICAL",
  "recipient_type": "USER|GROUP|ALL",
  "recipient_id": "string",
  "sender_id": "string",
  "template_id": "string (optional)",
  "payload": "object (optional)",
  "scheduled_at": "ISO 8601 timestamp (optional)",
  "expires_at": "ISO 8601 timestamp (optional)"
}
```

**Response:**
```json
{
  "id": "string",
  "title": "string",
  "content": "string",
  "type": "string",
  "priority": "string",
  "recipient_type": "string",
  "recipient_id": "string",
  "sender_id": "string",
  "created_at": "ISO 8601 timestamp",
  "updated_at": "ISO 8601 timestamp"
}
```

### Delete Notification
```
DELETE /notifications/{id}
```

**Response:**
```json
{
  "success": true
}
```

### List Notifications
```
GET /notifications
```

**Query Parameters:**
- `recipient_id` (string, optional): Filter by recipient ID
- `type` (string, optional): Filter by notification type
- `priority` (string, optional): Filter by priority level
- `limit` (integer, optional, default: 100): Maximum number of results
- `offset` (integer, optional, default: 0): Offset for pagination

**Response:**
```json
[
  {
    "id": "string",
    "title": "string",
    "content": "string",
    "type": "string",
    "priority": "string",
    "recipient_type": "string",
    "recipient_id": "string",
    "sender_id": "string",
    "created_at": "ISO 8601 timestamp",
    "updated_at": "ISO 8601 timestamp"
  }
]
```

### Send Notification
```
POST /notifications/{id}/send
```

**Response:**
```json
{
  "success": true
}
```

## Template Management

### Create Template
```
POST /templates
```

**Request Body:**
```json
{
  "name": "string",
  "description": "string",
  "subject_template": "string",
  "content_template": "string",
  "type": "INFO|WARNING|ERROR|SUCCESS",
  "priority": "LOW|NORMAL|HIGH|CRITICAL",
  "created_by": "string"
}
```

**Response:**
```json
{
  "id": "string",
  "name": "string",
  "description": "string",
  "subject_template": "string",
  "content_template": "string",
  "type": "string",
  "priority": "string",
  "created_by": "string",
  "is_active": true,
  "created_at": "ISO 8601 timestamp",
  "updated_at": "ISO 8601 timestamp"
}
```

### Get Template
```
GET /templates/{id}
```

**Response:**
```json
{
  "id": "string",
  "name": "string",
  "description": "string",
  "subject_template": "string",
  "content_template": "string",
  "type": "string",
  "priority": "string",
  "created_by": "string",
  "is_active": true,
  "created_at": "ISO 8601 timestamp",
  "updated_at": "ISO 8601 timestamp"
}
```

### Update Template
```
PUT /templates/{id}
```

**Request Body:**
```json
{
  "name": "string",
  "description": "string",
  "subject_template": "string",
  "content_template": "string",
  "type": "INFO|WARNING|ERROR|SUCCESS",
  "priority": "LOW|NORMAL|HIGH|CRITICAL",
  "is_active": "boolean"
}
```

**Response:**
```json
{
  "id": "string",
  "name": "string",
  "description": "string",
  "subject_template": "string",
  "content_template": "string",
  "type": "string",
  "priority": "string",
  "created_by": "string",
  "is_active": true,
  "created_at": "ISO 8601 timestamp",
  "updated_at": "ISO 8601 timestamp"
}
```

### Delete Template
```
DELETE /templates/{id}
```

**Response:**
```json
{
  "success": true
}
```

### List Templates
```
GET /templates
```

**Query Parameters:**
- `active_only` (boolean, optional, default: true): Filter by active status
- `limit` (integer, optional, default: 100): Maximum number of results
- `offset` (integer, optional, default: 0): Offset for pagination

**Response:**
```json
[
  {
    "id": "string",
    "name": "string",
    "description": "string",
    "subject_template": "string",
    "content_template": "string",
    "type": "string",
    "priority": "string",
    "created_by": "string",
    "is_active": true,
    "created_at": "ISO 8601 timestamp",
    "updated_at": "ISO 8601 timestamp"
  }
]
```

## Analytics

### Get Delivery Statistics
```
GET /analytics/delivery
```

**Response:**
```json
{
  "total_sent": 1000,
  "total_delivered": 950,
  "total_failed": 50,
  "total_read": 800,
  "delivery_rate": 95.0,
  "read_rate": 84.2,
  "failure_rate": 5.0
}
```

### Get Delivery History
```
GET /analytics/history
```

**Query Parameters:**
- `limit` (integer, optional, default: 100): Maximum number of results

**Response:**
```json
[
  {
    "notification_id": "string",
    "recipient_id": "string",
    "status": "SENT|DELIVERED|FAILED|READ",
    "failure_reason": "string (optional)",
    "timestamp": "ISO 8601 timestamp"
  }
]
```

## Health Check

### Server Health
```
GET /health
```

**Response:**
```json
{
  "status": "healthy",
  "timestamp": "ISO 8601 timestamp",
  "version": "1.0.0"
}
```

## Error Responses

All error responses follow this format:
```json
{
  "error": "string",
  "message": "string",
  "code": "integer"
}
```

### Common Error Codes:
- 400: Bad Request
- 401: Unauthorized
- 403: Forbidden
- 404: Not Found
- 500: Internal Server Error

## WebSocket Interface

The notification server also provides real-time notifications via WebSocket at:
```
ws://localhost:8080/ws
```

After connecting, clients can subscribe to notifications by sending a JSON message:
```json
{
  "action": "subscribe",
  "recipient_id": "user123"
}
```

The server will then push notifications to the client as they are delivered.
